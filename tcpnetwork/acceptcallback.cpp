#include "acceptcallback.h"

#include "dataiocallback.h"
#include "../common/packet.h"
#include "../common/pkg.h"
#include "../common/networkmsg.h"
#include <string>
AcceptCallback::AcceptCallback(){
	camera_clients_.clear();
	windows_clients_.clear();
	GetSnsByFile("sn.txt",need_collects_sn_);
	pthread_mutex_init(&camera_mutex_,NULL);
	pthread_mutex_init(&windows_mutex_,NULL);
}

AcceptCallback::~AcceptCallback() {
	pthread_mutex_destroy(&camera_mutex_);
	pthread_mutex_destroy(&windows_mutex_);
}

bool AcceptCallback::accept_callback(aio_socket_stream* client)
{
	ClientStruct *cs = new ClientStruct;
	memset(cs, 0, sizeof(ClientStruct));
	if(!extractclient(client->get_peer(true),client->get_local(true),*cs))
	{
		return false;
	}
	if(cs->serverport == 9326)
	{
		cs->type = CLINET_TYPE_CAMERA;
	}
	else if(cs->serverport == 8439)
	{
		cs->type = CLINET_TYPE_CLIENT;
	}
	else
	{
		cs->type = CLINET_TYPE_NULL;
	}
	DataIoCallback* callback = new DataIoCallback(client,this,cs->type);
	client->add_read_callback(callback);
	client->add_write_callback(callback);
	client->add_close_callback(callback);
	client->add_timeout_callback(callback);
	cs->callback = callback;
	if(cs->serverport == 9326)
	{
		pthread_mutex_lock(&camera_mutex_);
		camera_clients_.push_back(cs);
		pthread_mutex_unlock(&camera_mutex_);

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
		client->write(buffercapture,PkgLen(&sendpacketcapture));
		client->write(buffervideo,PkgLen(&sendpacketvideo));

		char regver[8] = {0};
		int size = PackGetCameraID(12, regver);
		client->write(regver,size);
	}
	else if(cs->serverport == 8439)
	{
		pthread_mutex_lock(&windows_mutex_);
		windows_clients_.push_back(cs);
		pthread_mutex_unlock(&windows_mutex_);
	}
	else
	{
	}
	client->read(8,15,0,NULL);
	printf("A New Client Have connected[%s:%s,%d]\n",cs->serverport == 9326 ? "Camera":"Windows client",cs->clientip,cs->clientport);
	printf("Current client num [camera=%d],[windows clients=%d]\n",(int)camera_clients_.size(),(int)windows_clients_.size());
	return true;
}

bool AcceptCallback::UpdateClientInfo(const ClientStruct client)
{
	ClientStruct* target = NULL;
	if(client.type == CLINET_TYPE_CAMERA)
	{
		pthread_mutex_lock(&camera_mutex_);
		for (list<ClientStruct*>::iterator it = camera_clients_.begin(); it != camera_clients_.end(); ++it)
		{
			ClientStruct* each = *it;
			if (strcmp(each->clientip, client.clientip) == 0 && each->clientport == client.clientport)
			{
				target = each;
				break;
			}
		}
		pthread_mutex_unlock(&camera_mutex_);
	}
	else if(client.type == CLINET_TYPE_CLIENT)
	{
		pthread_mutex_lock(&windows_mutex_);
		for (list<ClientStruct*>::iterator it = windows_clients_.begin(); it != windows_clients_.end(); ++it)
		{
			ClientStruct* each = *it;
			if (strcmp(each->clientip, client.clientip) == 0 && each->clientport == client.clientport)
			{
				target = each;
				break;
			}
		}
		pthread_mutex_unlock(&windows_mutex_);
	}
	else
	{

	}
	if(target != NULL)
	{
		memcpy(target->cameraid,client.cameraid,32);
		memcpy(target->addr_id,client.addr_id,32);
		memcpy(target->addr_name,client.addr_name,96);
		memcpy(target->resv,client.resv,96);
		return true;
	}
	else
	{
		return false;
	}
}
void AcceptCallback::DeleteClientInfo(const ClientStruct client)
{
	if(client.type == CLINET_TYPE_CAMERA)
	{
		pthread_mutex_lock(&camera_mutex_);
		for (list<ClientStruct*>::iterator it = camera_clients_.begin(); it != camera_clients_.end(); ++it)
		{
			ClientStruct* each = *it;
			if (strcmp(each->clientip, client.clientip) == 0 && each->clientport == client.clientport)
			{
				delete (*it);
				it = camera_clients_.erase(it);
				break;
			}
		}
		pthread_mutex_unlock(&camera_mutex_);
	}
	else if(client.type == CLINET_TYPE_CLIENT)
	{
		pthread_mutex_unlock(&windows_mutex_);
		for (list<ClientStruct*>::iterator it = windows_clients_.begin(); it != windows_clients_.end();)
		{
			ClientStruct* each = *it;
			if (strcmp(each->clientip, client.clientip) == 0 && each->clientport == client.clientport)
			{
				delete (*it);
				it = windows_clients_.erase(it);
				break;
			}
			it++;
		}
		pthread_mutex_unlock(&windows_mutex_);
	}
	else
	{

	}
}

bool AcceptCallback::FindClientInfo(const ClientStruct client,vector<ClientStruct> &clint)
{
	clint.clear();
	if(client.type == CLINET_TYPE_CAMERA)
	{
		pthread_mutex_lock(&camera_mutex_);
		for (list<ClientStruct*>::iterator it = camera_clients_.begin(); it != camera_clients_.end(); ++it)
		{
			if (strcmp((*it)->cameraid, client.cameraid) == 0)
			{
				clint.push_back(*(*it));
			}
		}
		pthread_mutex_unlock(&camera_mutex_);
	}
	else if(client.type == CLINET_TYPE_CLIENT)
	{
		pthread_mutex_lock(&windows_mutex_);
		for (list<ClientStruct*>::iterator it = windows_clients_.begin(); it != windows_clients_.end(); ++it)
		{
			if (strcmp((*it)->cameraid, client.cameraid) == 0)
			{
				clint.push_back(*(*it));
			}
		}
		pthread_mutex_unlock(&windows_mutex_);
	}
	else
	{

	}
	return clint.size()>0;
}

int AcceptCallback::GetCameraList(vector<ClientStruct> &clint,int offset,int count,char *id,char *addrid,char *addrname,char *custom)
{
	clint.clear();
	offset = offset-1;
	int total = 0;
	if(offset < camera_clients_.size())
	{
		int con = 0;
		int offset_index = 0;
		pthread_mutex_lock(&camera_mutex_);
		for (list<ClientStruct*>::iterator it = camera_clients_.begin() ; it != camera_clients_.end(); ++it)
		{
			std::string tempid = (*it)->cameraid;
			std::string tempaddrid = (*it)->addr_id;
			std::string tempaddrname = (*it)->addr_name;
			std::string tempcustom = (*it)->addr_name;
			if(tempid.find(id) == std::string::npos)
				continue;
			if(tempaddrid.find(addrid) == std::string::npos)
				continue;
			if(tempaddrname.find(addrname) == std::string::npos)
				continue;
			if(tempcustom.find(custom) == std::string::npos)
				continue;
			if(offset_index >= offset && offset_index-offset <= count)
			{
				clint.push_back(*(*it));
				con++;
			}
			offset_index++;
			total++;
		}
		pthread_mutex_unlock(&camera_mutex_);
	}
	return total;
}
int AcceptCallback::GetSize(const ClientStruct client)
{
	int size = 0;
	if(client.type == CLINET_TYPE_CAMERA)
	{
		pthread_mutex_lock(&camera_mutex_);
		for (list<ClientStruct*>::iterator it = camera_clients_.begin(); it != camera_clients_.end(); ++it)
		{
			if (strcmp((*it)->cameraid, client.cameraid) == 0)
			{
				size++;
			}
		}
		pthread_mutex_unlock(&camera_mutex_);
	}
	else if(client.type == CLINET_TYPE_CLIENT)
	{
		pthread_mutex_lock(&windows_mutex_);
		for (list<ClientStruct*>::iterator it = windows_clients_.begin(); it != windows_clients_.end(); ++it)
		{
			if (strcmp((*it)->cameraid, client.cameraid) == 0)
			{
				size++;
			}
		}
		pthread_mutex_unlock(&windows_mutex_);
	}
	else
	{

	}
	return size;
}

void AcceptCallback::DisConnNoHeat(){

}

bool AcceptCallback::IsExistSn(const char *sn){
  bool is_find = false;
  for(int i=0; i< need_collects_sn_.size(); i++){
    if(need_collects_sn_[i].compare(sn) == 0){
      is_find = true;
      break;
    }
  }
  return is_find;
}

bool AcceptCallback::extractclient(const char *client,const char *server,ClientStruct &cs)
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
	std::string serverstr = server;
	size_t serverfound = serverstr.find_first_of(":");
	if(serverfound == std::string::npos)
	{
		return false;
	}
	strcpy(cs.serverip,serverstr.substr(startpos, serverfound-startpos).c_str());
	cs.serverport = atoi(serverstr.substr(serverfound+1, serverstr.size()).c_str());
	return true;
}

void AcceptCallback::GetSnsByFile(const char *file,vector<std::string> &vects){
  vects.clear();
  FILE *fin=fopen(file,"r");
  if(fin == NULL){
    return ;
  }
  char a[1024];
  while(fgets(a,1024,fin)){
    if(a[strlen(a) - 1] == '\n'){
      a[strlen(a) -1] = '\0';
    }
    vects.push_back(a);
  }
  fclose(fin);
}
