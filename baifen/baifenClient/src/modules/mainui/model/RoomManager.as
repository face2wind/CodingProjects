package modules.mainui.model
{
	import face2wind.event.ParamEvent;
	import face2wind.event.PropertyChangeEvent;
	import face2wind.manager.EventManager;
	
	import flash.events.EventDispatcher;
	import flash.utils.Dictionary;
	
	import modules.mainui.event.RoomEvent;
	
	import socketCommand.customData.RoomInfo;

	/**
	 * 房间数据管理器
	 * @author face2wind
	 */
	public class RoomManager extends EventDispatcher
	{
		public function RoomManager()
		{
			if(instance)
				throw new Error("RoomManager is singleton class and allready exists!");
			instance = this;
			
			_roomDic = new Dictionary();
		}
		
		/**
		 * 单例
		 */
		private static var instance:RoomManager;
		/**
		 * 获取单例
		 */
		public static function getInstance():RoomManager
		{
			if(!instance)
				instance = new RoomManager();
			
			return instance;
		}
		
		/**
		 * 主角所在房间号
		 */
		private var _curRoomID:int;
		/**
		 * 主角所在房间号
		 */
		public function get curRoomID():int
		{
			return _curRoomID;
		}
		/**
		 * 主角所在房间号
		 */
		public function set curRoomID(value:int):void
		{
			if(_curRoomID == value)
				return;
			
			var _oldValue:* = _curRoomID;
			_curRoomID = value;
			dispatchEvent(PropertyChangeEvent.createUpdateEvent("curRoomID", _oldValue, _curRoomID));
		}		
		private var _roomDic:Dictionary;
		
		public function updateRoomList(roomList:Array):void
		{
			for (var i:int = 0; i < roomList.length; i++) 
			{
				var info:RoomInfo = roomList[i] as RoomInfo;
				_roomDic[info.roomID] = info;
			}
			EventManager.getInstance().dispatchToView(new ParamEvent(RoomEvent.ROOM_LIST_CHANGE));
		}
		
		public function updateRoom(info:RoomInfo):void
		{
			_roomDic[info.roomID] = info;
			EventManager.getInstance().dispatchToView(new ParamEvent(RoomEvent.ROOM_LIST_CHANGE));
		}
		
		public function getRoomInfo(roomID:int = 0):RoomInfo
		{
			if(0 == roomID)
				roomID = curRoomID;
			return _roomDic[roomID];
		}
		
		/**
		 * 主角当前所在的房间信息 
		 * @return 
		 */		
		public function get roleRoomVo():RoomVo
		{
			return _roomDic[curRoomID];
		}
	}
}