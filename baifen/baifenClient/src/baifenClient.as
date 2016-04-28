package
{
	import face2wind.ASBaseLib;
	import face2wind.ASUIComponent;
	import face2wind.customUIComponents.Message;
	import face2wind.event.ParamEvent;
	import face2wind.lib.Debuger;
	import face2wind.loading.QueueLoadingItem;
	import face2wind.loading.QueueResourceLoader;
	import face2wind.manager.EnterFrameUtil;
	import face2wind.manager.EventManager;
	import face2wind.net.GameSocket;
	import face2wind.net.SocketConnection;
	
	import flash.display.Sprite;
	import flash.utils.Endian;
	
	import modules.game.controller.GameController;
	import modules.login.controller.LoginController;
	import modules.mainui.controller.MainUIController;
	import modules.mainui.controller.RoomsController;
	
	import socketCommand.GameCommandMap;
	import socketCommand.c2s.cs100.CS10000;
	
	/**
	 * 百分主程序入口
	 * @author face2wind
	 */
	public class baifenClient extends Sprite
	{
		public function baifenClient()
		{
			ASBaseLib.initialize(this, onLoadMainCompleteHandler);
			ASUIComponent.initialize();
		}
		
		/**
		 * 初始化完毕 
		 */		
		private function onLoadMainCompleteHandler():void
		{
			Debuger.setModuleDebug(Debuger.SOCKET,true);
			EventManager.getInstance().bindToBaseLib(Debuger.EVENT_DEBUGER_MSG, ondebugMsgHandler);
			
			var loader:QueueResourceLoader = new QueueResourceLoader();
			loader.addEventListener(QueueResourceLoader.ALL_LOAD_COMPLETE, onAllLoadCompleteHandler);
			
			var vo:QueueLoadingItem = new QueueLoadingItem();
			vo.resUrl = "asUIComponent.swf";
			loader.addLoadingItem(vo);
			
			vo = new QueueLoadingItem();
			vo.resUrl = "mainUI.swf";
			loader.addLoadingItem(vo);
			
			loader.startLoading();
		}
		
		private function ondebugMsgHandler(e:ParamEvent):void
		{
//			Message.show(e.param.msg);
		}
		
		/**
		 * 所有素材加载完毕 
		 * @param event
		 */		
		protected function onAllLoadCompleteHandler(event:ParamEvent):void
		{
			GameSocket.getInstance().cmdMap = new GameCommandMap();
			GameSocket.getInstance().endian = Endian.LITTLE_ENDIAN;
			GameSocket.getInstance().addEventListener(SocketConnection.CONNECTED, onSocketConnect);
			GameSocket.getInstance().connectTo("192.168.1.104");
//			GameSocket.getInstance().connectTo("192.168.9.188");
			//			GameSocket.getInstance().connectTo("127.0.0.1");
		}
		
		/**
		 * 协议连接成功 
		 * @param event
		 */		
		protected function onSocketConnect(event:ParamEvent):void
		{
			LoginController.getInstance();
			MainUIController.getInstance();
			RoomsController.getInstance();
			GameController.getInstance();
		}
	}
}