#ifndef _HIS_OPCODE_H
#define _HIS_OPCODE_H

enum Opcodes
{
	MSG_NULL_ACTION = 0x0000,
	
	MSG_TEST = 501,
	/// ǰ500����Ϣ�������ڲ�ʹ��
	SMSG_ERROR = 502,                    /// ��������ͻ�������һ������

	/// �����Ϣ
	CMSG_PLAYER_ENTER_MAP = 505,         /// �ͻ��˷��ͽ�ɫ���뵱ǰ�ݶ�
	SMSG_PLAYER_CREATE_SELF,              /// ������֪ͨ�ͻ��˴�����ҽ�ɫ
	SMSG_PLAYER_CREATE_OTHER,             /// ������֪ͨ�ͻ��˴���������ҽ�ɫ
	SMSG_ROLE_REMOVE,		              /// ������֪ͨ�ͻ���ɾ����ɫ
	CMSG_PLAYER_RELIVE,                   /// �ͻ���֪ͨ��Ҹ���ѡ��
	SMSG_ROLE_ATTR_UPDATE,				  /// ������֪ͨ�ͻ����޸�����
	
	CMSG_PLAYER_MOVE,                     /// �ͻ�����������ƶ�
	SMSG_PLAYER_MOVE,                     /// ������֪ͨ�ͻ�������ƶ�

	CMSG_PLAYER_LEAVE_WORLD,              /// �ͻ����˳�
	CMSG_PLAYER_BEAT,                     /// ���������

	SMSG_PLAYER_UPGRADE,                  /// �������  
	SMSG_KICK_PLAYER,                     /// �߳����
	SMSG_ROLE_ASPECT_UPDATE,              /// �����۸�����Ϣ
	
	CMSG_PLAYER_LEAVE_MAP,                /// �ͻ���֪ͨ����������뿪��ǰ��ͼ
	SMSG_PLAYER_LEAVE_MAP,                /// ������֪ͨ�ͻ�����ҿ����뿪��ǰ��ͼ  

	SMSG_PLAYER_RELIVE,                   /// ������֪ͨ�ͻ��˸���
	CMSG_PLAYER_REST,                     /// ��ҿ�ʼ����

	CMSG_PLAYER_REACH_MAP,                /// �ͻ���֪ͨ����������Ѿ�����Ŀ���ͼ������Ϣ��Ӧ SMSG_PLAYER_LEAVE_MAP��

	/// end

	/// ��ͼ����������Ĺ㲥��Ϣ
	SMSG_ADDR_FAILURE,                    /// �㲥��ҵ�½��ͼ��������Ϣ 
	SMSG_ROUTED_ERROR,                    /// Ŀ��·�ɴ���

	/// ������Ϣ
	CMSG_TEAM_INVITE = 550,              /// ��ҷ���һ���������
	SMSG_TEAM_INVITE,                     /// �������������߷����������
	CMSG_TEAM_INVITE_ASK,                 /// �������߻ش��������
	SMSG_TEAM_INVITE_ASK,                 /// ���ת�����������Ƿ����Э��

	CMSG_TEAM_MEMBER_JOIN,                /// �ͻ����������һ������
	SMSG_TEAM_MEMBER_JOIN,                /// ������֪ͨ��Ҽ���/�˳�һ������
	CMSG_TEAM_MEMBER_JOIN_ASK,            /// �ͻ���֪ͨ�������ܾ�����ͬ��ĳ����Ҽ����Ŷ�
	SMSG_TEAM_MEMBER_JOIN_ASK,            /// ������֪ͨ��������Ŷӱ��ܾ����߽���
	SMSG_TEAM_MEMBER_LIST,                /// ��������ͻ��˷�����ҵĶ����Ա�б�
	SMSG_TEAM_MEMBER_CHANGE,              /// ������֪ͨһ������ĳ�Ա�����仯
	SMSG_TEAM_ERROR,					  /// ������֪ͨ��ӹ����еĴ�����Ϣ
	SMSG_TEAM_JOIN_ACK,					   ///��Ա����ȷ��
	SMSG_TEAM_INVITE_ADD_ACK,
	SMSG_TEAM_INVITE_ADDMEMBER,				/// ������ӳɹ�֪ͨ�������߼������

	CMSG_TEAM_KICT_PLAYER,					///�޳���Ա
	CMSG_TEAM_CHANGE_LEADER,				///�ͻ���֪ͨת���ӳ�
	SMSG_TEAM_CHANGE_LEADER,				///������֪ͨת���ӳ�
	SMSG_TEAM_CHANGE_LEADER_NOTIFY,			///��������֪ͨת���ӳ�
	SMSG_TEAM_CHANGE_LEADER_ACK,			///ת���ӳ�ȷ��
	SMSG_TEAM_SWAP_LEADER,
	CMSG_TEAM_LEAVE,						///leave the team
	SMSG_TEAM_LEAVE,
	SMSG_TEAM_LEAVE_NOTIFY,
	SMSG_TEAM_LINE,							///��Ա������
	SMSG_TEAM_ATTRIBUTE_UPDATE,				///duiyuan ���Ը���
	CMSG_TEAM_INVITE_NAME,				  /// ���ͨ�����ַ���һ���������
	CMSG_TEAM_MEMBER_JOIN_NAME,			  /// �ͻ���ͨ�������������һ������

	SMSG_TEAM_MONSTER_ASSIGN,				///ɱ�����������Ϣ

	CMSG_TEAM_ASSIGN_TYPE,					///���÷��䷽ʽ
	SMSG_TEAM_ASSIGN_TYPE,
	CMSG_TEAM_QUALITY,						///����Ʒ��
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

	/// ������Ϣ
	SMSG_MONSTER_CREATE,          /// ������֪ͨ�ͻ��˴�������
	SMSG_MONSTER_REMOVE,		        /// ������֪ͨ�ͻ���ɾ������
	SMSG_MONSTER_MOVE,					/// ������֪ͨ�ͻ��˹����ƶ�
	/// -end

	/// ��Ʒ��Ϣ
	CMSG_ITEM_DESTROY  = 600,                  /// �ͻ��˸�����������������Ʒ��Ϣ
	SMSG_ITEM_DESTROY,                  /// ���������ͻ��˷�����Ϣ��Ʒ��Ϣ
	CMSG_ITEM_MOVE,                     /// �ͻ���֪ͨ��������ҵ���Ʒ�ƶ�λ��
    CMSG_ITEM_SPLIT,                    /// ��Ʒ��� 
	SMSG_ITEM_UPDATE,                   /// ��Ʒ���Ա仯
	SMSG_ITEM_CREATE,                   /// ���������������Ʒ������Ϣ
	CMSG_ITEM_APPLY,                    /// �ͻ�������Ʒ�ϵ���Ҽ�
	SMSG_ITEM_CHANGE_EQUIPMENT,         /// ������֪ͨ�ͻ�������Ѿ��滻װ��
	SMSG_ITEM_BIND_CHOOSE,              /// ������֪ͨ�ͻ�����Ʒ��ѡ��
	CMSG_ITEM_BIND_ITEM,                /// �ͻ���֪ͨ��������Ʒʹ�ð�

    SMSG_ITEM_CREATE_DROPBOX,           /// ���Ӵ���
    SMSG_ITEM_DESTROY_DROPBOX,          /// ��������
    CMSG_ITEM_OPEN_DROPBOX,             /// �ͻ�������������ʹ�����
    CMSG_ITEM_CLOSE_DROPBOX,            /// �ͻ�������������͹ر�����
    SMSG_ITEM_DROP_LIST,                /// ��������ͻ��˷���������Ʒ�嵥
    CMSG_ITEM_PICKUP,                   /// �ͻ����������������Ʒʰȡ 
    SMSG_ITEM_PICKUP,                   /// ��������ͻ��˷�����Ʒʰȡ
    SMSG_ITEM_RAND,                     /// ��������ͻ��˷��͸߼���Ʒ���
    CMSG_ITEM_WANTED,                   /// �ͻ���������������Ƿ���Ҫ�߼���Ʒ
	SMSG_ITEM_WANTED,					/// ˭Roll�˶��ٵ�
	SMSG_ITEM_ASSIGN,					/// ��Ʒ�������˭
	SMSG_ITEM_COOL,						/// ��Ʒ��ȴ
	CMSG_ITEM_TIP,						/// Tip��ʾ��Ʒ
	SMSG_ITEM_TIP,						/// Tip��ʾ��Ʒ
	SMSG_REQ_ITEM_TIP,
	CMSG_ITEM_JUGDE,					/// ��Ʒ����
	CMSG_ITEM_REFRESH_ATTR,				/// ��Ʒ�������ˢ��
	CMSG_ITEM_REFRESH_SOCKET,			/// ��Ʒ��ˢ��
	CMSG_ITEM_ADD_SOCKET,				/// ��Ʒ������
	CMSG_ITEM_ADD_RUNE,					/// ��ʯ��Ƕ
	CMSG_ITEM_POP_RUNE,					/// ��ʯ��ж
	CMSG_ITEM_FIX,						/// װ������
	CMSG_ITEM_REFINE,					/// װ��ǿ��
	CMSG_ITEM_DISMANTLE,				/// װ�����
	SMSG_ITEM_DISMANTLE,				/// ����˷��ز��װ���ĵõ���Ʒ
	CMSG_ITEM_MAKE,						/// װ�����
	CMSG_BAG_CLEAN_UP,					/// ��������
	SMSG_BAG_CLEAN_UP,					/// ��������
	CMSG_WAREHOUSE_CLEAN_UP,			/// �ֿ�����
	SMSG_WAREHOUSE_CLEAN_UP,			/// �ֿ�����
	SMSG_ITEM_GET,						/// �õ���Ʒ������Ϣ

	/// -end

	///- ������Ϣ 
	SMSG_SPELL_COOL = 700,              /// ������ȴ
	SMSG_SKILL_MOVE,					/// ����˲���ƶ�

	SMSG_SPELL_READY_SELF,
	SMSG_SPELL_READY_COORD,
	SMSG_SPELL_READY_CRITTER,

	SMSG_SPELL_RELEASE_SELF,
	CMSG_SPELL_RELEASE_SELF,			  /// ���Լ�Ϊ���ļ����ͷ�
	SMSG_SPELL_RELEASE_COORD,
	CMSG_SPELL_RELEASE_COORD,
	SMSG_SPELL_RELEASE_CRITTER,
	CMSG_SPELL_RELEASE_CRITTER,
	
	CMSG_SKILL_LEARN,                     /// ����ѧϰ
	SMSG_BUFFER_SET,					  /// BUFFER����	
	SMSG_SKILL_SET,						  /// SKILL����
	SMSG_SKILL_MODIFY_ATTR,				  /// ���ܶ����Ե��޸�
	SMSG_SKILL_SHOW,					  /// ��������
	/// -end

	///- ������Ϣ  
	CMSG_TASK_ACCEPT = 750,             /// ����ͻ��������������Э��
	SMSG_TASK_CREATE,                     /// ����������������񴴽���Ϣ
	CMSG_TASK_TRACK_CHANGE,               /// ����ͻ�������׷������
	SMSG_TASK_TRACK_CHANGE,               /// �����������ͻ�������׷������
	SMSG_TASK_UPDATE,                     /// �����������ͻ�������״̬����
	SMSG_TASK_READY,                      /// �����������ͻ�����������ɾ���
	CMSG_TASK_ACCOMPLISH,                 /// ����ͻ�����������Э��
	SMSG_TASK_COMPLETE,                   /// ����������������������Ϣ 
	CMSG_TASK_ITEM,                       /// �ͻ��˷��ͻ�ȡ����������
	SMSG_TASK_ITEM,                       /// ����������taskѡ���б�
	CMSG_TASK_QUERY,                      /// �ͻ��˷��������ѯ����
	SMSG_TASK_QUERY,                      /// ����˷��ؿͻ��������ѯ����
	CMSG_TASK_QUERY_CAN_ACCEPT,           /// �ͻ��˷��Ͳ�ѯȫ���ɽ���������
	SMSG_TASK_QUERY_CAN_ACCEPT,           /// �����������ͻ���ȫ���ɽ�����

	CMSG_TASK_DROP,						  /// �ͷ��˷��ͷ���������Ϣ
	SMSG_TASK_DROP,
	/// -end

	///- NPC��Ϣ  
	SMSG_NPC_STATE,                /// ���ͻ��˷���npc״̬�䶯��Ϣ
	CMSG_NPC_STATE,                       /// �ͻ��˷���npc״̬������Ϣ
	/// -end
 

	///��½����Ϣ -start
	CMSG_CREATE_ACCOUNT = 800,		///�����˻�
	CMSG_LOGIN_ACCOUNT,					//��֤�˻�
	CMSG_LOGIN_ACCOUNT_Req,				//��֤�˻�����
	CMSG_CHANGE_PW,				        //�����˻�����
	SMSG_ACCOUNT_RESULT,				//�˻��������
	SMSG_LOGINREQUEST_REPLAY,		    //�ͷ��˵�¼���󷵻�
	SMSG_AUTH_SUCCESS,				    //�˻���֤�ɹ�
	GATEWAY_MSG_UPDATE,					///������Ϣ������Ϣ
	LOGIN_TOGATEWAY_AUTHMSG,			///��½��������֤��Ϣ������
	///-end
	
	///gate��Ϣ
	SMSG_GATE_REGISTER, 		        /// GS����Gateʱע��Map ID��Ϣ	
	CMSG_VALIATION_ACCOUNT,				///�ͻ�������gate���͵���֤��Ϣ
	SMSG_REQUEST_ROLE_LIST,				///����������role list
	
	CMSG_CHECK_ROLE_NAME,				///�ͻ��˼���ɫ������Ϣ
	SMSG_CHECK_ROLE_NAME,				///�������˷��ؽ�ɫ���Ƽ��ͨ����Ϣ
	CMSG_ROLE_MAKE,						///�ͻ��˷��ͽ�ɫ������Ϣ
	SMSG_ROLE_MAKE,						///���������ؽ�ɫ������Ϣ
	CMSG_REQUEST_ROLE_LIST,				///�ͻ��˷��������ɫ�б���Ϣ
	CMSG_ROLE_DELETE,					///�ͻ���ɾ����ɫ
	SMSG_ROLE_DELETE,					///�������˷���ɾ����ɫ�ɹ���Ϣ
	CMSG_TEST_TIME_INTERVAL,			///������Ϣ��ʱ���ӳ�
	
	CMSG_LOOPBACK,						    /// ѹ�����԰�
	GMSG_REPORT_ONLINE_PLAYERS,			///�������������û���login
	GMSG_REPORT_PLAYER_STATE,			///��ɫ��������֪ͨlogin

	/// ������Ϣ
	CMSG_CHAT,              /// �ͻ��˷���������Ϣ
	TMSG_CHAT,              /// ת��������Ϣ
	SMSG_CHAT,              /// ��������ͻ��˷���������Ϣ

	/// GM ָ��
	CMSG_GM_COMMAND,          /// GMָ��

    /// -������Ϣ
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

	/// ������Ϣ
	CMSG_SOCIAL_APPEND = 1000,
	SMSG_SOCIAL_NOTIFY_BE_APPEND,
	SMSG_SOCIAL_NOTIFY_ACK_BE_APPEND,
	SMSG_SOCIAL_APPEND,
	CMSG_SOCIAL_DELETE,
	SMSG_SOCIAL_DELETE,
	SMSG_SOCIAL_NOTIFY_ONLINE,
	SMSG_SOCIAL_NOTIFY_MODIFY,
	SMSG_SOCIAL_UPDATE_ONLINE,

	/// ������Ϣ
	CMSG_ENEMY_APPEND,
	SMSG_ENEMY_APPEND,
	CMSG_ENEMY_DELETE,
	SMSG_ENEMY_DELETE,
	SMSG_ENEMY_NOTIFY_ONLINE,
	SMSG_ENEMY_NOTIFY_MODIFY,
	SMSG_ENEMY_UPDATE_ONLINE,

    /// -end
	
	/// ��������Ϣ
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

	/// �����Ϣ
	CMSG_WING_STATUS_UPDATE, /// ���״̬����
	CMSG_WING_TRANSPORT,     /// ʹ�ó�����
	/// -end transcript message
	

	/// begin ������Ϣ
	CMSG_GUILD_CREATE = 1200,				// c��������

	CMSG_GUILD_OBTAIN,						// ��ȡ�Լ��Ĺ���(�򿪹������ʱ)

	SMSG_GUILD_UPDATE,						// �������(����������Ϣ�ͳ�Ա�б�)

	CMSG_GUILD_INVITE,						// ��������c
	SMSG_GUILD_INVITE,						// ��������s
	CMSG_GUILD_INVITE_REPLY,				// �������뷴��c
	SMSG_GUILD_INVITE_REPLY,				// �������뷴��s

	CMSG_GUILD_INVITE_BYNAME,				// ͨ�������������c

	CMSG_GUILDLIST_QUERY,					// ��ѯ�����б�(��������)C
	SMSG_GUILDLIST_RETURN,					// ���ع����б�S

	CMSG_GUILD_DETAIL_GET,					// ��ȡ������ϸ��Ϣ
	SMSG_GUILD_DETAIL_RETURN,				// ���ع�����ϸ��Ϣ

	CMSG_GUILD_APPLYJOIN,					// ������빫��c
	SMSG_GUILD_APPLYJOIN,					// ������빫��s
	CMSG_GUILD_APPLYJOIN_REPLY,				// ������빫�ᷴ��c
	SMSG_GUILD_APPLYJOIN_REPLY,				// ������빫�ᷴ��s

	CMSG_GUILD_APPLYJOIN_BYNAME,			// ͨ�����������������c


	SMSG_GUILD_MEMBER_JOINED,				// �³�Ա����֪ͨ
	
	CMSG_APPLYLIST_QUERY,					// ��ѯ�����б�(�����б�id)
	SMSG_APPLYLIST_UPDATE,					// �����б���

	CMSG_GUILD_KICK_MEMBER,					//C�޳���Ա
	SMSG_GUILD_MEMBER_KICKED_NOTIFY,		//S�޳���Ա

	CMSG_GUILD_CHANGE_LEADER_REQ,			//C���᳤����
	SMSG_GUILD_CHANGE_LEADER_REQ,			//S���᳤����
	CMSG_GUILD_CHANGE_LEADER_REPLY,			//C���᳤����
	SMSG_GUILD_CHANGE_LEADER_REPLY,			//S���᳤����
	SMSG_GUILD_LEADER_CHANGED_NOTIFY,		//S���᳤֪ͨÿ����Ա
	

	CMSG_GUILD_MEMBER_LEAVE,				//c�뿪����
	SMSG_GUILD_MEMBER_LEFT_NOTIFY,			//s�뿪����֪ͨ

	CMSG_GUILD_SET_ANNOUNCEMENT,			//CS�޸Ĺ���
	SMSG_GUILD_ANNOUNCEMENT_UPDATE,			//SC�������
	
	CMSG_GUILD_CHANGE_POSITION,				//CS����ְλ
	SMSG_GUILD_POSITION_CHANGED,			//SCְλ���

	CMSG_GUILD_DISBAND,						//CS��ɢ����
	SMSG_GUILD_DISBANDED,					//SC�����ɢ

	CMSG_GUILD_DISBAND_CANCEL,				//CSȡ����ɢ
	SMSG_GUILD_DISBAND_CANCELED,			//SC��ɢ��ȡ��

	SMSG_GUILD_MEMBER_ONLINE_NOTIFY,		//SC��Ա������֪ͨ
	SMSG_GUILD_MEMBER_UPGRADE,				//SC��Ա����֪ͨ

	SMSG_GUILD_DISBAND_TIME_OVER,			//SC��ɢʱ�䵽,������ʽ��ɢ

	NMSG_GUILD_UPDATE,						//SS֪ͨ����gs���±��ع���
	NMSG_GUILD_ADD,							//SS֪ͨ����gs���±��ع���
	NMSG_GUILD_REMOVE,						//SS֪ͨ����gs���±��ع���

	NMSG_GUILDMGR_UPDATE,					//SS֪ͨ����gs���±��ع���

	NMSG_GUILD_MEMBER_LEFT,					//֪ͨ������������뿪����(�������)
	NMSG_GUILD_ANNOUNCEMENT_CHANGED,		//֪ͨ����������ṫ����(����㲥)

	/// end ������Ϣ
	
	CMSG_OFFLINE_PRACT = 1300,              // ��������                      
	CMSG_ONLINE_PRACT,                      // ��������

	/// end �������

	MSG_END_CODE
};

#endif