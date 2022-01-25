/**
 * @file    common/networkmsg.h
 * @brief   华安网络通信协议消息ID定义。
 * @version 1.0.0
 * @date    2017-3-27
 * @author  陈超
 */

#if !defined(_COMMON_NETWORKMSG_H)
#define _COMMON_NETWORKMSG_H


#ifdef __cplusplus
extern "C" {
#endif

/// 网络消息
typedef enum {
  /*! 重启机器。 */
  MESSAGE_ID_REBOOT                    = 0,

  /*! 重启进程。 */
  MESSAGE_ID_RESTART                   = 1,

  /*! 心跳。 */
  MESSAGE_ID_HEART                     = 2,

  /*! 系统状态信息。 */
  MESSAGE_ID_SYSTEM_STATUS             = 3,

  /*! 统一应答。 */
  MESSAGE_ID_REPLY                     = 4,

  /*! 抓拍结果。 */
  MESSAGE_ID_RESULT                    = 5,

  /*! 查询版本信息。 */
  MESSAGE_ID_REQ_VERSION               = 6,

  /*! 登录认证。 */
  MESSAGE_ID_AUTH                      = 7,

  MESSAGE_ID_SERIAL_READ               = 8,      /* 透明串口 - 读 */
  MESSAGE_ID_SERIAL_READEx             = 80,
  MESSAGE_ID_SERIAL_WRITE              = 9,      /* 透明串口 - 写 */

  /*! 应用状态。 */
  MESSAGE_ID_APP_STATUS                = 10,

  /*! 请求系统时间。 */
  MESSAGE_ID_REQ_TIME                  = 11,

  /*! 修改系统时间。 */
  MESSAGE_ID_SET_TIME                  = 12,

  /*! 请求NTP信息。 */
  MESSAGE_ID_REQ_NTP                   = 13,

  /*! 修改NTP信息。 */
  MESSAGE_ID_SET_NTP                   = 14,

  /*! 请求系统网络信息。 */
  MESSAGE_ID_REQ_NETWORK               = 15,

  /*! 修改系统网络信息。 */
  MESSAGE_ID_SET_NETWORK               = 16,

  /*! 系统诊断信息。 */
  MESSAGE_ID_REQ_DIAGNOSE              = 17,

  /*!30 以下保留了系统级命令。 */
  MESSAGE_ID_REQ_CAMERALIST            = 20,
  MESSAGE_ID_REQ_CONNECT               = 21,
  MESSAGE_ID_REQ_CAMERAID              = 22,
  MESSAGE_ID_REQ_CAMERADISCONNECTED    = 23,

  /*! 实时图像传输开关。 */
  MESSAGE_ID_LIVE_IMAGE_ONOFF          = 102,

  /*! 实时图像。 */
  MESSAGE_ID_LIVE_IMAGE                = 103,

  /*! 算法中间结果。 */
  MESSAGE_ID_SEMI_RESULT               = 104,

  /*! 实时图像。 */
  MESSAGE_ID_LIVE_IMAGE2               = 105,

  /*! GPIO测试。 */
  MESSAGE_ID_GPIO_TESTING              = 110,

  /*! 测试播放音频 */
  MESSAGE_ID_TEST_PLAY_AUDIO           = 111,

  /*! GPIO输入信号。 */
  MESSAGE_ID_GPIO_INPUT                = 112,

  /*! 获取音频文件列表 */
  MESSAGE_ID_GET_AUDIO_LIST            = 113,

  /*! 获取TF卡存储信息*/
  MESSAGE_ID_GET_TF_CARD_INFO          = 123,

  /*! 格式化TF卡*/
  MESSAGE_ID_FORMAT_TF_CARD            = 124,

  /*! 播放音频流。 */
  MESSAGE_ID_PLAY_AUDIO_STREAM         = 125,

  /*! 查询应用程序配置信息(配置文件)。 */
  MESSAGE_ID_REQ_APP_CONFIG            = 201,

  /*! 设置应用程序配置信息。 */
  MESSAGE_ID_SET_APP_CONFIG            = 202,

  /*! 重置应用程序配置信息。 */
  MESSAGE_ID_RESET_APP_CONFIG          = 203,

  /*! 查询流（相机）参数。 */
  MESSAGE_ID_REQ_STREAM_CONFIG         = 204,

  /*! 设置流（相机）参数。 */
  MESSAGE_ID_SET_STREAM_CONFIG         = 205,

  /*! 重置流（相机）参数。 */
  MESSAGE_ID_RESET_STREAM_CONFIG       = 206,

  /*! 查询系统概略信息。 */
  MESSAGE_ID_REQ_SYS_COMPENDIUM_INFO   = 207,

  /*! 设置系统概略信息。 */
  MESSAGE_ID_SET_SYS_COMPENDIUM_INFO   = 208,

  /*! 重置系统概略信息。 */
  MESSAGE_ID_RESET_SYS_COMPENDIUM_INFO = 209,

  /*! 快照/截图。 */
  MESSAGE_ID_SNAPSHOT                  = 210,

  /*! 查询调度时间表。 */
  MESSAGE_ID_REQ_SCHEDULE_TABLE        = 211,

  /*! 修改调度时间表。 */
  MESSAGE_ID_SET_SCHEDULE_TABLE        = 212,

  /*! 浏览人像抓拍记录。 */
  MESSAGE_ID_BROWSE_FACE_RECORD        = 217,

  /*! 删除人像抓拍记录。 */
  MESSAGE_ID_DELETE_FACE_RECORD        = 219,
  
  /*! 调试图像信息。 */
  MESSAGE_ID_DEBUG_IMAGE               = 301,

  /*! 删除进度。 */
  MESSAGE_ID_FACE_DEL_PROGRESS         = 302,

  /*! 重注册进度。 */
  MESSAGE_ID_RE_REGISTER_PROGRESS      = 303,
  
  /*! 双目红外调试图像信息。 */
  MESSAGE_ID_DEBUG_BINOCULAR_IMAGE     = 304,

  /* 查询所有人像数据的ID*/
  MESSAGE_ID_QUERY_PERSONS_ID_DATA     = 400,
  
  /*! 浏览人像比对数据。 */
  MESSAGE_ID_BROWSE_FACE_FEATURE_DATA  = 401,

  /*! 添加人像比对数据。 */
  MESSAGE_ID_ADD_FACE_FEATURE_DATA     = 402,

  /*! 修改人像比对数据。 */
  MESSAGE_ID_MODIFY_FACE_FEATURE_DATA  = 403,

  /*! 删除人像比对数据。 */
  MESSAGE_ID_DELETE_FACE_FEATURE_DATA  = 404,

  /*! 查询人像比对数据。 */
  MESSAGE_ID_QUERY_FACE_FEATURE_DATA   = 405,

  /*! 开闸信息（抓拍机向外推送）。 */
  MESSAGE_ID_CTRL_RECORD               = 406,

  /*! 开闸/报警请求。 */
  MESSAGE_ID_CTRL_REQUEST              = 407,

  /*! PTZ控制。 */
  MESSAGE_ID_PTZ_CTRL                  = 501,

  /*! 读取用户校验码（用户第二次校验）。 */
  MESSAGE_ID_READ_AUTH_CODE            = 506,

  /*! 注入用户校验码（用户第二次校验）。 */
  MESSAGE_ID_WRITE_AUTH_CODE           = 507,

  /*! 计算双目摄像头像差。 */
  MESSAGE_ID_CAM_PIXEL_DIFF            = 508,

  /*! 获取双目摄像头像差信息。 */
  MESSAGE_ID_CAM_PIXEL_DIFF_INFO       = 509,

  /*! 系统功能授权。 */
  MESSAGE_ID_SYSTEM_FUNC_AUTH          = 510,

  /*! 查询系统功能授权。 */
  MESSAGE_ID_QUERY_FUNC_AUTH           = 511,

  /*! 查询对端系统信息，常用于获取对端基本信息以初始化本端某些元素（如界面）。 */
  MESSAGE_ID_REQ_SYS_INFO              = 1000,

  /// 外部协议。
  /*! 查询流（相机）参数。 */
  MESSAGE_ID_OUTER_REQ_CAMERA          = 801,

  /*! 修改流（相机）参数。 */
  MESSAGE_ID_OUTER_SET_CAMERA          = 802,

  /*! 重置流（相机）参数。 */
  MESSAGE_ID_OUTER_RESET_CAMERA        = 803,

  /*! 查询应用参数。 */
  MESSAGE_ID_OUTER_REQ_APP             = 804,

  /*! 修改应用参数。 */
  MESSAGE_ID_OUTER_SET_APP             = 805,

  /*! 重置应用参数。 */
  MESSAGE_ID_OUTER_RESET_APP           = 806
} MessageId;


/// 统一应答码的定义
typedef enum {
  /*!-------------- 0~1000为各系统通用应答码------------*/

  /*! 已成功处理消息。 */
  REPLYCODE_OK                = 0,

  /*! 所接收消息与本地协议不匹配。 */
  REPLYCODE_PROTOCOL_UNMATCH  = 1,

  /*! 无法识别的消息ID。 */
  REPLYCODE_MESSAGEID_ERROR   = 2,

  /*! 消息内容中存在错误的值。 */
  REPLYCODE_ERROR_VALUE       = 3,

  /*! 未通过认证。 */
  REPLYCODE_ERROR_AUTH        = 4,

  /*! 系统繁忙。 */
  REPLYCODE_ERROR_SYS_BUSY    = 5,

  /*! 资源不足。 */
  REPLYCODE_ERROR_LACK_RES    = 6,

  /*! 系统功能授权失败*/
  REPLYCODE_ERROR_FUNC_AUTH   = 7,

  /*! 系统功能已授权*/
  REPLYCODE_ERROR_FUNC_AUTHORIZED = 8,

  /*! 数据录入达到上限。 */
  REPLYCODE_ERROR_OVER_MAX    = 20,

  /*! 记录已经存在。 */
  REPLYCODE_ERROR_EXISTED     = 21,

  /*! 记录不存在。 */
  REPLYCODE_ERROR_NOT_EXIST   = 22,

  /*! 写入数据失败。 */
  REPLYCODE_ERROR_WRITE       = 23,

  /*! 读取数据失败。 */
  REPLYCODE_ERROR_READ        = 24,

  /*! 提取特征失败。 */
  REPLYCODE_ERR_FEATURE       = 25,

  /*! 人像质量差（Q值过低）。 */
  REPLYCODE_ERR_QVALUE        = 26,
  
  /*! WG卡号不支持模糊查找*/
  REPLYCODE_ERR_WG_QUERY_MODE = 30,



  /*!-------------1000~2000人脸抓拍系统使用-----------*/

  /*! 未检出指定个数可见光目标*/
  REPLYCODE_ERR_VISIBLE_TARGET                = 1001,

  /*! 未检出指定个数红外光目标*/
  REPLYCODE_ERR_INFRARED_TARGET               = 1002,

  /*! 可见光红外光像差过大*/
  REPLYCODE_ERR_ABERRATION_TOO_BIG            = 1003,

  /*! 无效的可见光图像*/
  REPLYCODE_ERR_VISIBLE_IMAGE                 = 1004,

  /*! 无效的红外光图像*/
  REPLYCODE_ERR_INFRARED_IMAGE                = 1005,

  /*! 特征数据版本与相机不匹配*/
  REPLYCODE_ERR_FEATURE_VERSION               = 1011,

  /*! 注册数据无归一化图像数据 */
  REPLYCODE_ERR_NO_TWIST_IMAGE                = 1012
} ReplyCode;

#define TL_HEADER_SIZE 8

#ifdef __cplusplus
}
#endif


#endif /* _COMMON_NETWORKMSG_H */
