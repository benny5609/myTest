#ifndef _HIS_OPCODE_H
#define _HIS_OPCODE_H

enum Opcodes
{
	MSG_NULL_ACTION = 0x0000,
	
	MSG_TEST = 501,
	/// 前500的消息保留给内部使用
	SMSG_ERROR = 502,                    /// 服务器向客户端描述一个错误

	/// 玩家消息
	CMSG_PLAYER_ENTER_MAP = 505,         /// 客户端发送角色进入当前梯度
	SMSG_PLAYER_CREATE_SELF,              /// 服务器通知客户端创建玩家角色
	SMSG_PLAYER_CREATE_OTHER,             /// 服务器通知客户端创建其他玩家角色
	SMSG_ROLE_REMOVE,		              /// 服务器通知客户端删除角色
	CMSG_PLAYER_RELIVE,                   /// 客户端通知玩家复活选择
	SMSG_ROLE_ATTR_UPDATE,				  /// 服务器通知客户端修改属性
	
	CMSG_PLAYER_MOVE,                     /// 客户端请求玩家移动
	SMSG_PLAYER_MOVE,                     /// 服务器通知客户端玩家移动

	CMSG_PLAYER_LEAVE_WORLD,              /// 客户端退出
	CMSG_PLAYER_BEAT,                     /// 玩家心跳包

	SMSG_PLAYER_UPGRADE,                  /// 玩家升级  
	SMSG_KICK_PLAYER,                     /// 踢出玩家
	SMSG_ROLE_ASPECT_UPDATE,              /// 玩家外观更新消息
	
	CMSG_PLAYER_LEAVE_MAP,                /// 客户端通知服务器玩家离开当前地图
	SMSG_PLAYER_LEAVE_MAP,                /// 服务器通知客户端玩家可以离开当前地图  

	SMSG_PLAYER_RELIVE,                   /// 服务器通知客户端复活
	CMSG_PLAYER_REST,                     /// 玩家开始打坐

	CMSG_PLAYER_REACH_MAP,                /// 客户端通知服务器玩家已经到达目标地图（此消息对应 SMSG_PLAYER_LEAVE_MAP）

	/// end

	/// 地图管理服务器的广播消息
	SMSG_ADDR_FAILURE,                    /// 广播玩家登陆地图服务器消息 
	SMSG_ROUTED_ERROR,                    /// 目标路由错误

	/// 队伍消息
	CMSG_TEAM_INVITE = 550,              /// 玩家发起一个组队邀请
	SMSG_TEAM_INVITE,                     /// 服务器向被邀请者发出组队邀请
	CMSG_TEAM_INVITE_ASK,                 /// 被邀请者回答组队邀请
	SMSG_TEAM_INVITE_ASK,                 /// 跨服转发被邀请者是否接受协议

	CMSG_TEAM_MEMBER_JOIN,                /// 客户端请求加入一个队伍
	SMSG_TEAM_MEMBER_JOIN,                /// 服务器通知玩家加入/退出一个队伍
	CMSG_TEAM_MEMBER_JOIN_ASK,            /// 客户端通知服务器拒绝或者同意某个玩家加入团队
	SMSG_TEAM_MEMBER_JOIN_ASK,            /// 服务器通知申请加入团队被拒绝或者接受
	SMSG_TEAM_MEMBER_LIST,                /// 服务器向客户端发送玩家的队伍成员列表
	SMSG_TEAM_MEMBER_CHANGE,              /// 服务器通知一个队伍的成员发生变化
	SMSG_TEAM_ERROR,					  /// 服务器通知组队过程中的错误信息
	SMSG_TEAM_JOIN_ACK,					   ///队员加入确认
	SMSG_TEAM_INVITE_ADD_ACK,
	SMSG_TEAM_INVITE_ADDMEMBER,				/// 邀请组队成功通知被邀请者加入队伍

	CMSG_TEAM_KICT_PLAYER,					///剔除队员
	CMSG_TEAM_CHANGE_LEADER,				///客户端通知转换队长
	SMSG_TEAM_CHANGE_LEADER,				///服务器通知转换队长
	SMSG_TEAM_CHANGE_LEADER_NOTIFY,			///服务器间通知转换队长
	SMSG_TEAM_CHANGE_LEADER_ACK,			///转换队长确认
	SMSG_TEAM_SWAP_LEADER,
	CMSG_TEAM_LEAVE,						///leave the team
	SMSG_TEAM_LEAVE,
	SMSG_TEAM_LEAVE_NOTIFY,
	SMSG_TEAM_LINE,							///队员上下线
	SMSG_TEAM_ATTRIBUTE_UPDATE,				///duiyuan 属性更新
	CMSG_TEAM_INVITE_NAME,				  /// 玩家通过名字发起一个组队邀请
	CMSG_TEAM_MEMBER_JOIN_NAME,			  /// 客户端通过名字请求加入一个队伍

	SMSG_TEAM_MONSTER_ASSIGN,				///杀死怪物分配消息

	CMSG_TEAM_ASSIGN_TYPE,					///设置分配方式
	SMSG_TEAM_ASSIGN_TYPE,
	CMSG_TEAM_QUALITY,						///设置品质
	SMSG_TEAM_QUALITY,

	SMSG_TEAM_OFFLINE_CHANGE_LEADER_NOTIFY,
	SMSG_TEAM_OFFLINE_CHANGE_LEADER_ACK,

	CMSG_TEAM_NOTE_LIST,
	SMSG_TEAM_NOTE_LIST,
	SMSG_TEAM_NOTE_LIST_UPDATE,
	CMSG_TEAM_NOTE_MEMBER,
	SMSG_TEAM_NOTE_MEMBER,
	CMSG_TEAM_NOTE_CREATE,
	/// end

	/// 怪物消息
	SMSG_MONSTER_CREATE,          /// 服务器通知客户端创建怪物
	SMSG_MONSTER_REMOVE,		        /// 服务器通知客户端删除怪物
	SMSG_MONSTER_MOVE,					/// 服务器通知客户端怪物移动
	/// -end

	/// 物品消息
	CMSG_ITEM_DESTROY  = 600,                  /// 客户端给服务器发送销毁物品消息
	SMSG_ITEM_DESTROY,                  /// 服务器给客户端发送消息物品消息
	CMSG_ITEM_MOVE,                     /// 客户端通知服务器玩家的物品移动位置
    CMSG_ITEM_SPLIT,                    /// 物品拆分 
	SMSG_ITEM_UPDATE,                   /// 物品属性变化
	SMSG_ITEM_CREATE,                   /// 服务器发送玩家物品创建信息
	CMSG_ITEM_APPLY,                    /// 客户端在物品上点击右键
	SMSG_ITEM_CHANGE_EQUIPMENT,         /// 服务器通知客户端玩家已经替换装备
	SMSG_ITEM_BIND_CHOOSE,              /// 服务器通知客户端物品绑定选择
	CMSG_ITEM_BIND_ITEM,                /// 客户端通知服务器物品使用绑定

    SMSG_ITEM_CREATE_DROPBOX,           /// 箱子创建
    SMSG_ITEM_DESTROY_DROPBOX,          /// 箱子销毁
    CMSG_ITEM_OPEN_DROPBOX,             /// 客户端向服务器发送打开箱子
    CMSG_ITEM_CLOSE_DROPBOX,            /// 客户端向服务器发送关闭箱子
    SMSG_ITEM_DROP_LIST,                /// 服务器向客户端发送箱子物品清单
    CMSG_ITEM_PICKUP,                   /// 客户端向服务器发送物品拾取 
    SMSG_ITEM_PICKUP,                   /// 服务器向客户端发送物品拾取
    SMSG_ITEM_RAND,                     /// 服务器向客户端发送高级物品随机
    CMSG_ITEM_WANTED,                   /// 客户端向服务器发送是否想要高级物品
	SMSG_ITEM_WANTED,					/// 谁Roll了多少点
	SMSG_ITEM_ASSIGN,					/// 物品分配给了谁
	SMSG_ITEM_COOL,						/// 物品冷却
	CMSG_ITEM_TIP,						/// Tip显示物品
	SMSG_ITEM_TIP,						/// Tip显示物品
	SMSG_REQ_ITEM_TIP,
	CMSG_ITEM_JUGDE,					/// 物品鉴定
	CMSG_ITEM_REFRESH_ATTR,				/// 物品随机属性刷新
	CMSG_ITEM_REFRESH_SOCKET,			/// 物品洞刷新
	CMSG_ITEM_ADD_SOCKET,				/// 物品洞增加
	CMSG_ITEM_ADD_RUNE,					/// 符石镶嵌
	CMSG_ITEM_POP_RUNE,					/// 符石拆卸
	CMSG_ITEM_FIX,						/// 装备修理
	CMSG_ITEM_REFINE,					/// 装备强化
	CMSG_ITEM_DISMANTLE,				/// 装备拆解
	SMSG_ITEM_DISMANTLE,				/// 服务端返回拆解装备的得的物品
	CMSG_ITEM_MAKE,						/// 装备拆解
	CMSG_BAG_CLEAN_UP,					/// 背包整理
	SMSG_BAG_CLEAN_UP,					/// 背包整理
	CMSG_WAREHOUSE_CLEAN_UP,			/// 仓库整理
	SMSG_WAREHOUSE_CLEAN_UP,			/// 仓库整理
	SMSG_ITEM_GET,						/// 得到物品数量消息

	/// -end

	///- 技能消息 
	SMSG_SPELL_COOL = 700,              /// 技能冷却
	SMSG_SKILL_MOVE,					/// 技能瞬间移动

	SMSG_SPELL_READY_SELF,
	SMSG_SPELL_READY_COORD,
	SMSG_SPELL_READY_CRITTER,

	SMSG_SPELL_RELEASE_SELF,
	CMSG_SPELL_RELEASE_SELF,			  /// 以自己为中心技能释放
	SMSG_SPELL_RELEASE_COORD,
	CMSG_SPELL_RELEASE_COORD,
	SMSG_SPELL_RELEASE_CRITTER,
	CMSG_SPELL_RELEASE_CRITTER,
	
	CMSG_SKILL_LEARN,                     /// 技能学习
	SMSG_BUFFER_SET,					  /// BUFFER设置	
	SMSG_SKILL_SET,						  /// SKILL设置
	SMSG_SKILL_MODIFY_ATTR,				  /// 技能对属性的修改
	SMSG_SKILL_SHOW,					  /// 技能显视
	/// -end

	///- 任务消息  
	CMSG_TASK_ACCEPT = 750,             /// 处理客户端请求接新任务协议
	SMSG_TASK_CREATE,                     /// 服务器发送玩家任务创建信息
	CMSG_TASK_TRACK_CHANGE,               /// 处理客户端任务追踪请求
	SMSG_TASK_TRACK_CHANGE,               /// 服务器发给客户端任务追踪请求
	SMSG_TASK_UPDATE,                     /// 服务器发给客户端任务状态更新
	SMSG_TASK_READY,                      /// 服务器发给客户端任务已完成就绪
	CMSG_TASK_ACCOMPLISH,                 /// 处理客户端请求交任务协议
	SMSG_TASK_COMPLETE,                   /// 服务器发送玩家任务完成信息 
	CMSG_TASK_ITEM,                       /// 客户端发送获取任务项请求
	SMSG_TASK_ITEM,                       /// 服务器发送task选项列表
	CMSG_TASK_QUERY,                      /// 客户端发送任务查询请求
	SMSG_TASK_QUERY,                      /// 服务端返回客户端任务查询请求
	CMSG_TASK_QUERY_CAN_ACCEPT,           /// 客户端发送查询全部可接任务请求
	SMSG_TASK_QUERY_CAN_ACCEPT,           /// 服务器发给客户端全部可接任务

	CMSG_TASK_DROP,						  /// 客服端发送放弃任务消息
	SMSG_TASK_DROP,
	/// -end

	///- NPC消息  
	SMSG_NPC_STATE,                /// 给客户端发送npc状态变动消息
	CMSG_NPC_STATE,                       /// 客户端发送npc状态请求消息
	/// -end
 

	///登陆服消息 -start
	CMSG_CREATE_ACCOUNT = 800,		///创建账户
	CMSG_LOGIN_ACCOUNT,					//验证账户
	CMSG_LOGIN_ACCOUNT_Req,				//验证账户请求
	CMSG_CHANGE_PW,				        //更改账户密码
	SMSG_ACCOUNT_RESULT,				//账户操作结果
	SMSG_LOGINREQUEST_REPLAY,		    //客服端登录请求返回
	SMSG_AUTH_SUCCESS,				    //账户验证成功
	GATEWAY_MSG_UPDATE,					///网关消息更新消息
	LOGIN_TOGATEWAY_AUTHMSG,			///登陆服发送验证信息给网关
	///-end
	
	///gate消息
	SMSG_GATE_REGISTER, 		        /// GS连接Gate时注册Map ID消息	
	CMSG_VALIATION_ACCOUNT,				///客户端连接gate发送的验证消息
	SMSG_REQUEST_ROLE_LIST,				///服务器返回role list
	
	CMSG_CHECK_ROLE_NAME,				///客户端检查角色名称消息
	SMSG_CHECK_ROLE_NAME,				///服务器端返回角色名称检查通过消息
	CMSG_ROLE_MAKE,						///客户端发送角色创建消息
	SMSG_ROLE_MAKE,						///服务器返回角色创建消息
	CMSG_REQUEST_ROLE_LIST,				///客户端发送请求角色列表消息
	CMSG_ROLE_DELETE,					///客户端删除角色
	SMSG_ROLE_DELETE,					///服务器端返回删除角色成功消息
	CMSG_TEST_TIME_INTERVAL,			///测试消息包时间延迟
	
	CMSG_LOOPBACK,						    /// 压力测试包
	GMSG_REPORT_ONLINE_PLAYERS,			///报告所有在线用户给login
	GMSG_REPORT_PLAYER_STATE,			///角色上线下线通知login

	/// 聊天消息
	CMSG_CHAT,              /// 客户端发送聊天消息
	TMSG_CHAT,              /// 转发聊天消息
	SMSG_CHAT,              /// 服务器向客户端发送聊天消息

	/// GM 指令
	CMSG_GM_COMMAND,          /// GM指令

    /// -交易消息
    CMSG_TRADE_NPC_BUY = 900,             
    CMSG_TRADE_NPC_SELL,
    CMSG_TRADE_NPC_REACQUIRE,
    SMSG_TRADE_NPC_REACQUIRE,
    CMSG_TRADE_P2P_REQUEST,
    SMSG_TRADE_P2P_REQUEST,
    CMSG_TRADE_P2P_RESPONSE,
    SMSG_TRADE_P2P_OPEN,
    CMSG_TRADE_P2P_ARRANGE,
    SMSG_TRADE_P2P_ARRANGE,	
    CMSG_TRADE_P2P_CRYSTAL,
    SMSG_TRADE_P2P_CRYSTAL,
    CMSG_TRADE_P2P_LOCK,
    SMSG_TRADE_P2P_LOCK,
    CMSG_TRADE_P2P_CONFIRM,
    SMSG_TRADE_P2P_CONFIRM,
    CMSG_TRADE_P2P_CLOSE,
    SMSG_TRADE_P2P_CLOSE,
	CMSG_TRADE_STALLAGE_READY,
	SMSG_TRADE_STALLAGE_READY,
	CMSG_TRADE_STALLAGE_SELL,
	SMSG_TRADE_STALLAGE_SELL,
	CMSG_TRADE_STALLAGE_CLOSE,
	SMSG_TRADE_STALLAGE_CLOSE,
	CMSG_TRADE_STALLAGE_VIEW,
	SMSG_TRADE_STALLAGE_VIEW,
	CMSG_TRADE_STALLAGE_BUY,
	SMSG_TRADE_STALLAGE_BUY,
	CMSG_TRADE_SAVE_MONEY,
	CMSG_TRADE_TAKE_MONEY,

	/// 好友消息
	CMSG_SOCIAL_APPEND = 1000,
	SMSG_SOCIAL_NOTIFY_BE_APPEND,
	SMSG_SOCIAL_NOTIFY_ACK_BE_APPEND,
	SMSG_SOCIAL_APPEND,
	CMSG_SOCIAL_DELETE,
	SMSG_SOCIAL_DELETE,
	SMSG_SOCIAL_NOTIFY_ONLINE,
	SMSG_SOCIAL_NOTIFY_MODIFY,
	SMSG_SOCIAL_UPDATE_ONLINE,

	/// 仇人消息
	CMSG_ENEMY_APPEND,
	SMSG_ENEMY_APPEND,
	CMSG_ENEMY_DELETE,
	SMSG_ENEMY_DELETE,
	SMSG_ENEMY_NOTIFY_ONLINE,
	SMSG_ENEMY_NOTIFY_MODIFY,
	SMSG_ENEMY_UPDATE_ONLINE,

    /// -end
	
	/// 黑名单消息
	CMSG_BLACKLIST_APPEND,
	SMSG_BLACKLIST_APPEND,
	CMSG_BLACKLIST_DELETE,
	SMSG_BLACKLIST_DELETE,
	/// -end

	/// transcript message
	CMSG_TRANSCRIPT_REQUEST = 1100,
	SMSG_TRANSCRIPT_PREPARE,
	SMSG_TRANSCRIPT_READY,  //send jump map message when transcript is ready, gs send this message to plugin when gs(transcript) starts up
	SMSG_TRANSCRIPT_KICK,
	SMSG_TRANSCRIPT_QUEUE,
	SMSG_TRANSCRIPT_FINISH,

	SMSG_TRANSCRIPT_REPORT_NO,
	SMSG_TRANSCRIPT_CREATE,
	DMSG_TRANSCRIPT_READY,
	SMSG_TRANSCRIPT_LAST_TIME_NOTIFY,

	/// 翅膀消息
	CMSG_WING_STATUS_UPDATE, /// 翅膀状态更新
	CMSG_WING_TRANSPORT,     /// 使用翅膀飞翔
	/// -end transcript message
	

	/// begin 公会消息
	CMSG_GUILD_CREATE = 1200,				// c创建公会

	CMSG_GUILD_OBTAIN,						// 提取自己的公会(打开工会界面时)

	SMSG_GUILD_UPDATE,						// 公会更新(包括公会信息和成员列表)

	CMSG_GUILD_INVITE,						// 公会邀请c
	SMSG_GUILD_INVITE,						// 公会邀请s
	CMSG_GUILD_INVITE_REPLY,				// 公会邀请反馈c
	SMSG_GUILD_INVITE_REPLY,				// 公会邀请反馈s

	CMSG_GUILD_INVITE_BYNAME,				// 通过玩家名字邀请c

	CMSG_GUILDLIST_QUERY,					// 查询公会列表(用于申请)C
	SMSG_GUILDLIST_RETURN,					// 返回公会列表S

	CMSG_GUILD_DETAIL_GET,					// 获取公会详细信息
	SMSG_GUILD_DETAIL_RETURN,				// 返回公会详细信息

	CMSG_GUILD_APPLYJOIN,					// 申请加入公会c
	SMSG_GUILD_APPLYJOIN,					// 申请加入公会s
	CMSG_GUILD_APPLYJOIN_REPLY,				// 申请加入公会反馈c
	SMSG_GUILD_APPLYJOIN_REPLY,				// 申请加入公会反馈s

	CMSG_GUILD_APPLYJOIN_BYNAME,			// 通过公会名称申请加入c


	SMSG_GUILD_MEMBER_JOINED,				// 新成员加入通知
	
	CMSG_APPLYLIST_QUERY,					// 查询申请列表(根据列表id)
	SMSG_APPLYLIST_UPDATE,					// 申请列表返回

	CMSG_GUILD_KICK_MEMBER,					//C剔除成员
	SMSG_GUILD_MEMBER_KICKED_NOTIFY,		//S剔除成员

	CMSG_GUILD_CHANGE_LEADER_REQ,			//C换会长请求
	SMSG_GUILD_CHANGE_LEADER_REQ,			//S换会长请求
	CMSG_GUILD_CHANGE_LEADER_REPLY,			//C换会长反馈
	SMSG_GUILD_CHANGE_LEADER_REPLY,			//S换会长反馈
	SMSG_GUILD_LEADER_CHANGED_NOTIFY,		//S换会长通知每个成员
	

	CMSG_GUILD_MEMBER_LEAVE,				//c离开公会
	SMSG_GUILD_MEMBER_LEFT_NOTIFY,			//s离开公会通知

	CMSG_GUILD_SET_ANNOUNCEMENT,			//CS修改公告
	SMSG_GUILD_ANNOUNCEMENT_UPDATE,			//SC公告更新
	
	CMSG_GUILD_CHANGE_POSITION,				//CS更改职位
	SMSG_GUILD_POSITION_CHANGED,			//SC职位变更

	CMSG_GUILD_DISBAND,						//CS解散公会
	SMSG_GUILD_DISBANDED,					//SC公会解散

	CMSG_GUILD_DISBAND_CANCEL,				//CS取消解散
	SMSG_GUILD_DISBAND_CANCELED,			//SC解散已取消

	SMSG_GUILD_MEMBER_ONLINE_NOTIFY,		//SC成员上下线通知
	SMSG_GUILD_MEMBER_UPGRADE,				//SC成员升级通知

	SMSG_GUILD_DISBAND_TIME_OVER,			//SC解散时间到,公会正式解散

	NMSG_GUILD_UPDATE,						//SS通知其他gs更新本地公会
	NMSG_GUILD_ADD,							//SS通知其他gs更新本地公会
	NMSG_GUILD_REMOVE,						//SS通知其他gs更新本地公会

	NMSG_GUILDMGR_UPDATE,					//SS通知其他gs更新本地公会

	NMSG_GUILD_MEMBER_LEFT,					//通知其他插件有人离开公会(任务清除)
	NMSG_GUILD_ANNOUNCEMENT_CHANGED,		//通知其他插件公会公告变更(聊天广播)

	/// end 公会消息
	
	CMSG_OFFLINE_PRACT = 1300,              // 离线修炼                      
	CMSG_ONLINE_PRACT,                      // 在线修炼

	/// end 玩家修炼

	MSG_END_CODE
};

#endif