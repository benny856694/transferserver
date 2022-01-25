
#if !defined(_COMMON_PACKET_H)
#define _COMMON_PACKET_H



#ifdef __cplusplus
extern "C" {
#endif

int PackHeartBeat(int system_type, char data[8]);
int PackGetCameraID(int system_type, char data[8]);

#ifdef __cplusplus
}
#endif


#endif /* _COMMON_PACKET_H */
