package modules.mainui.controller
{
	import common.ErrorMessage;
	
	import face2wind.base.Controller;
	import face2wind.customUIComponents.Message;
	import face2wind.event.ParamEvent;
	
	import flash.utils.Dictionary;
	
	import modules.game.event.GameEvent;
	import modules.mainui.event.RoomEvent;
	import modules.mainui.model.RoomManager;
	
	import socketCommand.c2s.cs101.CS10100;
	import socketCommand.c2s.cs101.CS10101;
	import socketCommand.c2s.cs101.CS10102;
	import socketCommand.c2s.cs101.CS10103;
	import socketCommand.s2c.sc101.SC10100;
	import socketCommand.s2c.sc101.SC10101;
	import socketCommand.s2c.sc101.SC10102;
	import socketCommand.s2c.sc101.SC10103;
	import socketCommand.s2c.sc101.SC10104;
	
	/**
	 * 房间请求控制器
	 * @author face2wind
	 */
	public class RoomsController extends Controller
	{
		public function RoomsController()
		{
			if(instance)
				throw new Error("RoomsController is singleton class and allready exists!");
			instance = this;
			
			_page2handleDic = new Dictionary();
			manager = RoomManager.getInstance();
			
			addCmdListener(10100, on10100handle);
			addCmdListener(10101, on10101handle);
			addCmdListener(10102, on10102handle);
			addCmdListener(10103, on10103handle);
			addCmdListener(10104, on10104handle);
			
			eventManager.bindToController(RoomEvent.REQUEST_ROOM_LIST, onRequestRoomListHandler);
			eventManager.bindToController(RoomEvent.REQUEST_CREATE_ROOM, onCreateRoomHandler);
			eventManager.bindToController(RoomEvent.REQUEST_JOIN_ROOM, onRequestJoinRoomHandler);
			eventManager.bindToController(RoomEvent.REQUEST_EXIT_ROOM, onRequestExitRoomHandler);
		}
		
		/**
		 * 单例
		 */
		private static var instance:RoomsController;
		/**
		 * 获取单例
		 */
		public static function getInstance():RoomsController
		{
			if(!instance)
				instance = new RoomsController();
			
			return instance;
		}
		
		private var _page2handleDic:Dictionary;
		private var manager:RoomManager;
		
		private function onRequestRoomListHandler(e:ParamEvent):void
		{
			var page:int = e.param.page;
			var handle:Function = e.param.handle;
			var hasSend:Boolean = false;
			if(null == _page2handleDic[page])
				_page2handleDic[page] = [];
			else
				hasSend = true;
			if(null != handle)
				_page2handleDic[page].push(handle);
			if(!hasSend){
				var cmd:CS10100 = new CS10100();
				cmd.pageNum = page;
				sendMessage(10100, cmd);
			}
		}
		
		private function onCreateRoomHandler(e:ParamEvent):void
		{
			var cmd:CS10102 = new CS10102();
			cmd.lockType = 0;
			cmd.passwd = "";
			sendMessage(10102, cmd);
		}
		
		private function onRequestJoinRoomHandler(e:ParamEvent):void
		{
			var cmd:CS10101 = new CS10101();
			cmd.roomID = e.param.roomID;
			cmd.sitID = e.param.sitID;
			cmd.passwd = "";
			sendMessage(10101, cmd);
		}
		
		private function onRequestExitRoomHandler(e:ParamEvent):void
		{
			var cmd:CS10103 = new CS10103();
			cmd.roomID = e.param.roomID;
			cmd.sitID = e.param.sitID;
			sendMessage(10103, cmd);
		}
		
		// 协议接收 ========================================================
		/**
		 * 房间列表 
		 * @param sc
		 */		
		private function on10100handle(sc:SC10100):void
		{
			if(null == _page2handleDic[sc.pageNum])
				return;
			
			var handleList:Array = _page2handleDic[sc.pageNum];
			for (var i:int = 0; i < handleList.length; i++) 
				(handleList[i] as Function).apply(null, [sc.pageNum,sc.roomList]);
			manager.updateRoomList(sc.roomList);
			delete _page2handleDic[sc.pageNum];
		}
		
		/**
		 * 加入房间 
		 * @param sc
		 */		
		private function on10101handle(sc:SC10101):void
		{
			Message.show(ErrorMessage.findMsgWithCode(sc.result));
			if(10100 == sc.result){
				manager.curRoomID = sc.roomID;
				eventManager.dispatchToView(new ParamEvent(RoomEvent.ENTER_ROOM_SUCCESS));
			}
		}
		
		/**
		 * 建房间
		 
		 * @param sc
		 */		
		private function on10102handle(sc:SC10102):void
		{
			Message.show(ErrorMessage.findMsgWithCode(sc.result));
			if(10105 == sc.result){
				manager.curRoomID = sc.roomID;
				eventManager.dispatchToView(new ParamEvent(RoomEvent.ENTER_ROOM_SUCCESS));
			}
		}
		
		/**
		 * 踢出、自己退出房间
		 * @param sc
		 */		
		private function on10103handle(sc:SC10103):void
		{
			Message.show(ErrorMessage.findMsgWithCode(sc.result));
			if(10109 == sc.result){
				eventManager.dispatchToController(new ParamEvent(GameEvent.GAME_STATE_CHANGE, {state:0}));
				eventManager.dispatchToView(new ParamEvent(RoomEvent.EXIT_ROOM_SUCCESS));
			}
		}
		
		/**
		 * 踢出、自己退出房间
		 * @param sc
		 */		
		private function on10104handle(sc:SC10104):void
		{
			manager.updateRoom(sc.roomInfo);
		}
	}
}