#include "dataiocallback.h"
#include "../common/pkg.h"
#include "../common/networkmsg.h"
#include "../common/packet.h"
#include "acceptcallback.h"
#include "../utils/unix_time.h"
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>

static int save_file(const char* file, const void* data, int data_size) {
  int ret = -1;
  FILE* fp = fopen(file, "wb");
  if (fp) {
    fwrite(data, data_size, 1, fp);
    fclose(fp);

    ret = 0;
  }
  return ret;
}

static int create_directory(const char* dir) {
  int i, len;

  len = strlen(dir);
  char dir_path[4096];
  memset(dir_path, 0, sizeof(dir_path));
  strncpy(dir_path, dir, len);

  for (i = 0; i < len; i++) {
    if (dir_path[i] == '/' && i > 0) {
      dir_path[i] = '\0';
      if (access(dir_path, F_OK) < 0) {
        if (mkdir(dir_path, 0755) < 0) {
          return 0;
        }
      }
      dir_path[i] = '/';
    }
  }
  if (i == len) {
    if (access(dir_path, F_OK) < 0)
      if (mkdir(dir_path, 0755) < 0)
        return 0;
  }

  return 1;
}

static std::string GenImagePath(struct timeval* cap_time,char *sn,int type) {
  char image_path[256] = { 0 };
  snprintf(image_path, sizeof(image_path), "images/%04d-%02d-%02d/%s/%s/",time_year(cap_time), time_month(cap_time),time_mday(cap_time),sn,type==1 ? "Overall" : type==2 ? "AlgPic" : type == 3 ? "AlgPicReg":"Other");
  return image_path;
}

static std::string GenImageName(struct timeval* cap_time,const char *rootdir,char *id,int sequence,int type) {
  char image_name[512] = { 0 };
  if(type == 1){
  snprintf(image_name, sizeof(image_name), "%s%s_%d(%04d%02d%02d_%02d%02d%02d).jpg",
    rootdir,id,sequence,
    time_year(cap_time), time_month(cap_time), time_mday(cap_time),
    time_hour(cap_time), time_minute(cap_time), time_second(cap_time));
  }else if(type == 2){
    snprintf(image_name, sizeof(image_name), "%s%s.jpg",
        rootdir,id);
  }else if(type == 3){
    snprintf(image_name, sizeof(image_name), "%s%s.jpg",
        rootdir,id);
  }else{
    snprintf(image_name, sizeof(image_name), "%s%s_other.jpg",
            rootdir,id);
  }
  return image_name;
}

DataIoCallback::DataIoCallback(aio_socket_stream* client,AcceptCallback *mast,ClientType type)
: client_(client)
, mast_(mast)
, type_(type)
, pkg_type_(0)
, pkg_len_(0)
, new_pkg_(1){
	memset(id_,0,sizeof(id_));
}
DataIoCallback::~DataIoCallback(){

}
bool DataIoCallback::read_callback(char* data, int len){
	int sys_type;
	int major_protocol;
	int minor_protocol;
	int msg_id;
	if(new_pkg_ && len == 8)
	{
		Packet packet;
		InitPkg(&packet, (unsigned char*)data, len, little_endian);
		GetInt(&packet,&pkg_type_);
		GetType(pkg_type_, &sys_type, &major_protocol, &minor_protocol, &msg_id);
		GetInt(&packet,&pkg_len_);
//		printf("First msgid=[%d],type=%d,lenth=%d\n",msg_id,type_,pkg_len_);
		switch (msg_id)
		{
		    case MESSAGE_ID_HEART:
		    {
		    	char heartbeat[8] = { 0 };
		    	int size = PackHeartBeat(12, heartbeat);
		    	client_->write(heartbeat,size);
				new_pkg_ = 1;
				client_->read(8,15,0,NULL);
				return true;
		    	break;
		    }
		    default:
		    {
		    	break;
		    }
		}
		if(pkg_len_ > 0)
		{
			new_pkg_ = 0;
			client_->read(pkg_len_,15,0,NULL);
		}
		else if(pkg_len_ == 0)
		{
			sendtodata((unsigned char *)data,len);
			new_pkg_ = 1;
			client_->read(8,15,0,NULL);
		}
		else
		{
			printf("Protocol Error(>8)\n");
		}
	}
	else
	{
		if(pkg_len_ > 0)
		{
			GetType(pkg_type_, &sys_type, &major_protocol, &minor_protocol, &msg_id);
//			printf("Second msgid=[%d],lenth=%d\n",msg_id,pkg_len_);
			switch (msg_id)
			{
				case MESSAGE_ID_REQ_CAMERAID:
				{
					ClientStruct cs;
					memset(&cs,0,sizeof(ClientStruct));
					Packet packet;
					InitPkg(&packet, (unsigned char*)data, pkg_len_, little_endian);
					GetString(&packet,id_,32);
//					printf("Get camerid id=%s,%d\n",id_,type_);
					cs.type = type_;
					extractclientip(client_->get_peer(true),cs);
					memcpy(cs.cameraid,id_,32);
					GetString(&packet,cs.addr_id,32);
					GetString(&packet,cs.addr_name,96);
					GetBytes(&packet,(unsigned char *)cs.resv,128);
					mast_->UpdateClientInfo(cs);
					if(mast_->IsExistSn(id_)){
					  char buffercapture[32];
            Packet sendpacketcapture;
            InitPkg(&sendpacketcapture, (unsigned char*)buffercapture, sizeof(buffercapture), little_endian);
            int tye2 = CreateType(12,1,14,399);
            PutInt(&sendpacketcapture,tye2);
            PutInt(&sendpacketcapture,0);
            PutInt(&sendpacketcapture,1);
            PkgEnd(&sendpacketcapture);
            client_->write(buffercapture,PkgLen(&sendpacketcapture));
					}
					break;
				}
				case MESSAGE_ID_REQ_CONNECT:
				{
					Packet packet;
					InitPkg(&packet, (unsigned char*)data, pkg_len_, little_endian);
					GetString(&packet,id_,32);
//					printf("Get windows id=%s\n",id_);
					ClientStruct cs;
					cs.type = type_;
					extractclientip(client_->get_peer(true),cs);
					memcpy(cs.cameraid,id_,32);
					mast_->UpdateClientInfo(cs);
					ClientStruct csfind;
					if(type_ == CLINET_TYPE_CAMERA)
					{
						csfind.type = CLINET_TYPE_CLIENT;
					}
					else if(type_ == CLINET_TYPE_CLIENT)
					{
						csfind.type = CLINET_TYPE_CAMERA;
					}
					else
					{
						csfind.type = CLINET_TYPE_NULL;
					}
					strcpy(csfind.cameraid,id_);
					vector<ClientStruct> sendclient;
					mast_->FindClientInfo(csfind,sendclient);
					char buffer[64];
					Packet sendpacket;
					InitPkg(&sendpacket, (unsigned char*)buffer, sizeof(buffer), little_endian);
					int tye = CreateType(12,1,14,MESSAGE_ID_REPLY);
					PutInt(&sendpacket,tye);
					PutInt(&sendpacket,0);
					PutInt(&sendpacket,MESSAGE_ID_REQ_CONNECT);
					int ack = sendclient.size()>0 ? 0 : -1;
					PutInt(&sendpacket,ack);
					PkgEnd(&sendpacket);
					client_->write(buffer,PkgLen(&sendpacket));
					break;
				}
				case MESSAGE_ID_REQ_CAMERALIST:
				{
					int offset = 0;
					int count = 0;
					char sn[96] = {0};
					char addrid[96] = {0};
					char addrname[96] = {0};
					char custom[32] = {0};
					Packet packet;
					InitPkg(&packet, (unsigned char*)data, pkg_len_, little_endian);
					GetInt(&packet,&offset);
					GetInt(&packet,&count);
					GetString(&packet,sn,96);
					GetString(&packet,addrid,96);
					GetString(&packet,addrname,96);
					GetString(&packet,custom,32);
					vector<ClientStruct> cameralist;
					int total = mast_->GetCameraList(cameralist,offset,count,sn,addrid,addrname,custom);
					char *databuffer = new char[2*1024*1024];
					Packet sendpacket;
					InitPkg(&sendpacket, (unsigned char*)databuffer, 2*1024*1024, little_endian);
					int tye = CreateType(12,1,14,MESSAGE_ID_REPLY);
					PutInt(&sendpacket,tye);
					PutInt(&sendpacket,0);
					PutInt(&sendpacket,MESSAGE_ID_REQ_CAMERALIST);
					PutInt(&sendpacket,0);
					PutInt(&sendpacket,total);
					PutInt(&sendpacket,cameralist.size());
					char resv[198];
					for(int i=0;i<cameralist.size();i++)
					{
						PutString(&sendpacket,cameralist[i].cameraid,32);
						PutString(&sendpacket,cameralist[i].clientip,16);
						PutInt(&sendpacket,cameralist[i].clientport);
						PutString(&sendpacket,cameralist[i].addr_id,32);
						PutString(&sendpacket,cameralist[i].addr_name,96);
						PutBytes(&sendpacket,(unsigned char *)cameralist[i].resv,128);
					}
					PkgEnd(&sendpacket);
					client_->write(databuffer,PkgLen(&sendpacket));
					delete[] databuffer;
					databuffer = NULL;
					break;
				}
				case MESSAGE_ID_RESULT:{
				  Packet packet;
          InitPkg(&packet, (unsigned char*)data, pkg_len_, little_endian);
          int sequence = 0;
          GetInt(&packet,&sequence);
          Seek(&packet,170);
          short match_status;
          char person_id[20] = { 0 };
          GetShort(&packet, &match_status);
          if(match_status > 0){
            GetString(&packet, person_id, 20);
            Seek(&packet,20);
            int exist_overall = 0;
            int overall_size = 0;
            GetInt(&packet, &exist_overall);
            if(exist_overall){
              Seek(&packet,4);
              GetInt(&packet, &overall_size);
              Seek(&packet,8);
            }
            int exist_closeup = 0;
            GetInt(&packet, &exist_closeup);
            if(exist_closeup){
              Seek(&packet,16);
            }
            int exist_videos = 0;
            GetInt(&packet, &exist_videos);
            if(exist_videos){
              Seek(&packet,24);
            }
            Seek(&packet,128);
            if(exist_overall){
              char *overall_pic = new char[overall_size];
              GetBytes(&packet, (unsigned char *)overall_pic, overall_size);
              struct timeval now;
              gettimeofday(&now,NULL);
              SaveFileToFlash(&now,person_id,1,sequence,overall_pic,overall_size);
              delete[] overall_pic;
              overall_pic = NULL;
            }
          }
          char *databuffer = new char[pkg_len_ + 8];
          Packet sendpacket;
          InitPkg(&sendpacket, (unsigned char*)databuffer, pkg_len_ + 8, little_endian);
          PutInt(&sendpacket,pkg_type_);
          PutInt(&sendpacket,pkg_len_);
          PutBytes(&sendpacket,(unsigned char *)data,len);
          sendtodata((unsigned char *)databuffer,pkg_len_ + 8);
          delete[] databuffer;
          databuffer = NULL;
				  break;
				}
				case MESSAGE_ID_DEBUG_IMAGE:
				{
				  Packet packet;
          InitPkg(&packet, (unsigned char*)data, pkg_len_, little_endian);
          Seek(&packet,16);
          char person_id[20] = { 0 };
          GetString(&packet, person_id, 20);
          int image_num = 0;
          GetInt(&packet, &image_num);
          Seek(&packet,8);
          int face_len = 0;
          GetInt(&packet,&face_len);
          if(face_len > 0){
            char *face_pic = new char[face_len];
            GetBytes(&packet, (unsigned char *)face_pic, face_len);
            struct timeval now;
            gettimeofday(&now,NULL);
            SaveFileToFlash(&now,person_id,3,0,face_pic,face_len);
            delete[] face_pic;
            face_pic = NULL;
          }
          if(image_num == 2){
            Seek(&packet,8);
            int reg_face_len = 0;
            GetInt(&packet,&reg_face_len);
            if(reg_face_len > 0){
              char *reg_face_pic = new char[reg_face_len];
              GetBytes(&packet, (unsigned char *)reg_face_pic, reg_face_len);
              struct timeval now;
              gettimeofday(&now,NULL);
              SaveFileToFlash(&now,person_id,2,0,reg_face_pic,reg_face_len);
              delete[] reg_face_pic;
              reg_face_pic = NULL;
            }
          }
          char *databuffer = new char[pkg_len_ + 8];
          Packet sendpacket;
          InitPkg(&sendpacket, (unsigned char*)databuffer, pkg_len_ + 8, little_endian);
          PutInt(&sendpacket,pkg_type_);
          PutInt(&sendpacket,pkg_len_);
          PutBytes(&sendpacket,(unsigned char *)data,len);
          sendtodata((unsigned char *)databuffer,pkg_len_ + 8);
          delete[] databuffer;
          databuffer = NULL;
				  break;
				}
				default:
				{
					char *databuffer = new char[pkg_len_ + 8];
					Packet sendpacket;
					InitPkg(&sendpacket, (unsigned char*)databuffer, pkg_len_ + 8, little_endian);
					PutInt(&sendpacket,pkg_type_);
					PutInt(&sendpacket,pkg_len_);
					PutBytes(&sendpacket,(unsigned char *)data,len);
					sendtodata((unsigned char *)databuffer,pkg_len_ + 8);
					delete[] databuffer;
					databuffer = NULL;
					break;
				}
			}
			new_pkg_ = 1;
			client_->read(8,15,0,NULL);
		}
		else
		{
			printf("Error Happened pkg_len < 0 (%d)\n",pkg_len_);
		}
	}
	return true;
}
bool DataIoCallback::write_callback(){
	return true;
}
void DataIoCallback::close_callback(){
	ClientStruct cs;
	cs.type = type_;
	extractclientip(client_->get_peer(true),cs);
	mast_->DeleteClientInfo(cs);
	if(type_ == CLINET_TYPE_CAMERA)
	{
		ClientStruct csfind;
		csfind.type = CLINET_TYPE_CLIENT;
		strcpy(csfind.cameraid,id_);
		vector<ClientStruct> sendclient;
		mast_->FindClientInfo(csfind,sendclient);
		if(sendclient.size() > 0)
		{
			char buffer[32];
			Packet sendpacket;
			InitPkg(&sendpacket, (unsigned char*)buffer, sizeof(buffer), little_endian);
			int tye = CreateType(12,1,14,MESSAGE_ID_REQ_CAMERADISCONNECTED);
			PutInt(&sendpacket,tye);
			PutInt(&sendpacket,0);
			PkgEnd(&sendpacket);
			for(int i=0;i<sendclient.size();i++)
			{
				sendclient[i].callback->GetPeer()->write(buffer,PkgLen(&sendpacket));
			}
		}
	}
	else if(type_ == CLINET_TYPE_CLIENT)
	{
		ClientStruct csfind;
		csfind.type = CLINET_TYPE_CLIENT;
		strcpy(csfind.cameraid,id_);
		if(mast_->GetSize(csfind) == 0)
		{
			csfind.type = CLINET_TYPE_CAMERA;
			vector<ClientStruct> sendclient;
			mast_->FindClientInfo(csfind,sendclient);
			char buffervideo[32];
			Packet sendpacketvideo;
			InitPkg(&sendpacketvideo, (unsigned char*)buffervideo, sizeof(buffervideo), little_endian);
			int tye = CreateType(12,1,14,MESSAGE_ID_LIVE_IMAGE_ONOFF);
			PutInt(&sendpacketvideo,tye);
			PutInt(&sendpacketvideo,0);
			PutInt(&sendpacketvideo,0);
			PkgEnd(&sendpacketvideo);

			char buffercapture[32];
			Packet sendpacketcapture;
			InitPkg(&sendpacketcapture, (unsigned char*)buffercapture, sizeof(buffercapture), little_endian);
			int tye2 = CreateType(12,1,14,399);
			PutInt(&sendpacketcapture,tye2);
			PutInt(&sendpacketcapture,0);
			PutInt(&sendpacketcapture,0);
			PkgEnd(&sendpacketcapture);

			for(int i=0;i<sendclient.size();i++)
			{
				sendclient[i].callback->GetPeer()->write(buffervideo,PkgLen(&sendpacketvideo));
				if(!mast_->IsExistSn(id_)){
				  sendclient[i].callback->GetPeer()->write(buffercapture,PkgLen(&sendpacketcapture));
				}
			}
		}
	}
	else
	{
	}
	printf("A Client Have Disconnected[%s:%s,%d]\n",cs.serverport == 9326 ? "Camera":"Windows client",cs.clientip,cs.clientport);
	delete this;
}
bool DataIoCallback::timeout_callback(){
	printf("A Client TimeOut\n");
	return (false);
}
aio_socket_stream* DataIoCallback::GetPeer()
{
	return client_;
}
bool DataIoCallback::extractclientip(const char *client,ClientStruct &cs)
{
	int startpos = 0;
	std::string clientstr = client;
	size_t clientfound = clientstr.find_first_of(":");
	if(clientfound == std::string::npos)
	{
		return false;
	}
	strcpy(cs.clientip,clientstr.substr(startpos, clientfound-startpos).c_str());
	cs.clientport = atoi(clientstr.substr(clientfound+1, clientstr.size()).c_str());
	return true;
}
void DataIoCallback::sendtodata(unsigned char *data,int datalen)
{
	ClientStruct cs;
	if(type_ == CLINET_TYPE_CAMERA)
	{
		cs.type = CLINET_TYPE_CLIENT;
	}
	else if(type_ == CLINET_TYPE_CLIENT)
	{
		cs.type = CLINET_TYPE_CAMERA;
	}
	else
	{
		cs.type = CLINET_TYPE_NULL;
	}
	strcpy(cs.cameraid,id_);
	vector<ClientStruct> sendclient;
	if(mast_->FindClientInfo(cs,sendclient))
	{
		for(int i=0;i<sendclient.size();i++)
		{
			sendclient[i].callback->GetPeer()->write(data,datalen);
		}
	}
	else
	{

		if(type_ == CLINET_TYPE_CAMERA)
		{
			int sys_type;
			int major_protocol;
			int minor_protocol;
			int msg_id;
			Packet packet;
			InitPkg(&packet, (unsigned char*)data, datalen, little_endian);
			GetType(pkg_type_, &sys_type, &major_protocol, &minor_protocol, &msg_id);
			if(msg_id == MESSAGE_ID_LIVE_IMAGE || msg_id == MESSAGE_ID_LIVE_IMAGE2 || msg_id == 505 || msg_id == MESSAGE_ID_RESULT)
			{
				char buffervideo[32];
				Packet sendpacketvideo;
				InitPkg(&sendpacketvideo, (unsigned char*)buffervideo, sizeof(buffervideo), little_endian);
				int tye = CreateType(12,1,14,MESSAGE_ID_LIVE_IMAGE_ONOFF);
				PutInt(&sendpacketvideo,tye);
				PutInt(&sendpacketvideo,0);
				PutInt(&sendpacketvideo,0);
				PkgEnd(&sendpacketvideo);

				char buffercapture[32];
				Packet sendpacketcapture;
				InitPkg(&sendpacketcapture, (unsigned char*)buffercapture, sizeof(buffercapture), little_endian);
				int tye2 = CreateType(12,1,14,399);
				PutInt(&sendpacketcapture,tye2);
				PutInt(&sendpacketcapture,0);
				PutInt(&sendpacketcapture,0);
				PkgEnd(&sendpacketcapture);
				if(!mast_->IsExistSn(id_)){
				  client_->write(buffercapture,PkgLen(&sendpacketcapture));
				}
				client_->write(buffervideo,PkgLen(&sendpacketvideo));
			}
		}
	}
}

void DataIoCallback::SaveFileToFlash(struct timeval* cap_time,char *person_id, int type,int sequence,char *img,int img_len){
  std::string image_path = GenImagePath(cap_time,id_, type);
  if (!create_directory(image_path.c_str())) {
    printf("Create directory[%s] failed!\n", image_path.c_str());
    return;
  }
  std::string image_name = GenImageName(cap_time,image_path.c_str(),person_id,sequence, type);
  save_file(image_name.c_str(), img, img_len);
}
