#pragma once
#include "../acl_cpp_define.hpp"
#include <map>
#include <vector>
#if !defined(_WIN32) && !defined(_WIN64)
#include <sys/time.h>
#endif
#include "../stream/aio_socket_stream.hpp"                                
#include "../stdlib/string.hpp"

namespace acl
{

class check_timer;
class aio_socket_stream;

/**
 * �첽���ӻص�����������
 */
class ACL_CPP_API check_client : public aio_open_callback
{
public:
	check_client(check_timer& timer, const char* addr,
		aio_socket_stream& conn, struct timeval& begin);

	/**
	 * �������ķ����� IO ���
	 * @return {aio_socket_stream&}
	 */
	aio_socket_stream& get_conn() const
	{
		return conn_;
	}

	/**
	 * ��ô���ķ���˵�ַ
	 * @return {const char*}
	 */
	const char* get_addr() const
	{
		return addr_.c_str();
	}

	/**
	 * ���������Ƿ��Ǵ���
	 * @param yesno {bool}
	 */
	void set_alive(bool yesno);

	/**
	 * �رշ����� IO ���
	 */
	void close();

public:
	// ���µĺ��������ڲ�ʹ��
	/**
	 * ��ǰ�������Ƿ�������ģʽ��
	 * @return {bool}
	 */
	bool blocked() const
	{
		return blocked_;
	}

	/**
	 * ��������ⷽʽ�£����ô˺����������ü������Ƿ�������״̬��
	 * ��������״̬ʱ�ü������ǽ�ֹͨ�����÷��� close ���رյ�
	 * @param on {bool} ���ü������Ƿ�������״̬��ȱʡΪ��������״̬
	 */
	void set_blocked(bool on);

private:
	// �����麯��
	bool open_callback();
	void close_callback();
	bool timeout_callback();

private:
	~check_client() {}

private:
	bool blocked_;
	bool aliving_;
	bool timedout_;
	struct timeval begin_;
	check_timer& timer_;
	aio_socket_stream& conn_;
	string addr_;
};

} // namespace acl
