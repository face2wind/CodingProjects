package modules.mainui.view
{
	import cache.PlayerCache;
	
	import face2wind.customUIComponents.Message;
	import face2wind.event.ParamEvent;
	import face2wind.uiComponents.Button;
	import face2wind.uiComponents.CustomTextfield;
	import face2wind.view.BaseSprite;
	
	import flash.events.MouseEvent;
	import flash.text.TextFormatAlign;
	
	import modules.game.event.GameEvent;
	import modules.mainui.event.RoomEvent;
	import modules.mainui.model.RoomManager;
	
	import socketCommand.customData.RoomInfo;
	import socketCommand.customData.RoomInfo_playerList;
	
	/**
	 * 房间视图
	 * @author face2wind
	 */
	public class RoomView extends BaseSprite
	{
		public function RoomView()
		{
			super();
		}
		
		private var roomNum:CustomTextfield;
		private var playerNameList:Array;
		private var exitBtn:Button;
		private var startGameBtn:Button;
		
		/**
		 * 此函数是视图的内容初始化函数<br/>对父类的覆盖 
		 * 
		 */		
		protected override function createChildren():void
		{
			super.createChildren();
			
			graphics.beginFill(0xfff000);
			graphics.drawRoundRect(0,0,300,300,20);
			graphics.endFill();
						
			roomNum = new CustomTextfield();
			roomNum.move(100,120);
			roomNum.size = 18;
			roomNum.width = 100;
			roomNum.align = TextFormatAlign.CENTER;
			roomNum.text = "房间号";
			addChild(roomNum);
			
			playerNameList = [];
			var playerTxtPosition:Array = [[100,0],[0,100],[100,270],[250,100]];
			for (var i:int = 0; i < 4; i++) 
			{
				var nameTxt:CustomTextfield = new CustomTextfield();
				nameTxt.text = "空位";
				nameTxt.x = playerTxtPosition[i][0];
				nameTxt.y = playerTxtPosition[i][1];
				addChild(nameTxt);
				playerNameList.push(nameTxt);
			}
			
			exitBtn = new Button();
			exitBtn.label = "退出房间";
			exitBtn.move(300,270);
			addChild(exitBtn);
			
			startGameBtn = new Button();
			startGameBtn.label = "开始游戏";
			startGameBtn.move(200,310);
			addChild(startGameBtn);
		}
		
		protected override function update():void
		{
			super.update();
			var info:RoomInfo = RoomManager.getInstance().getRoomInfo();
			if(null == info)
				return;
			roomNum.text = info.roomID+"号房间";
			playerNameList[0].text = "空位";
			playerNameList[1].text = "空位";
			playerNameList[2].text = "空位";
			playerNameList[3].text = "空位";
			for (var i:int = 0; i < info.playerList.length; i++) 
			{
				var list:RoomInfo_playerList = info.playerList[i];
				var nameTxt:CustomTextfield = playerNameList[list.sitID];
				if(null == nameTxt)
					continue;
				nameTxt.text = list.playerName;
			}
		}
		
		private function onRoomListChange(e:ParamEvent):void
		{
			propertyChange();
		}
		
		protected function onRequestExitRoomHandler(event:MouseEvent):void
		{
			var info:RoomInfo = RoomManager.getInstance().getRoomInfo();
			var roleID:int = PlayerCache.getInstance().roleVo.id;
			var mySitID:int = -1;
			for (var i:int = 0; i < info.playerList.length; i++) 
			{
				var list:RoomInfo_playerList = info.playerList[i];
				if(list.playerID == roleID)
					mySitID = list.sitID;
			}
			if(-1 == mySitID)
				return;
			eventManager.dispatchToController(new ParamEvent(RoomEvent.REQUEST_EXIT_ROOM,
				{roomID:RoomManager.getInstance().curRoomID, sitID:mySitID }));
		}
		
		protected function onRequestStartGameHandler(event:MouseEvent):void
		{
			eventManager.dispatchToController(new ParamEvent(GameEvent.REQUEST_START_GAME));
		}
		
		/**
		 * 被加到显示列表时执行 
		 */		
		public override function onShowHandler():void
		{
			propertyChange();
			exitBtn.addEventListener(MouseEvent.CLICK, onRequestExitRoomHandler);
			startGameBtn.addEventListener(MouseEvent.CLICK, onRequestStartGameHandler);
			eventManager.bindToView(RoomEvent.ROOM_LIST_CHANGE, onRoomListChange);
		}
		
		/**
		 * 从显示列表移除时执行 
		 */		
		public override function onHideHandler():void
		{
			eventManager.unbindToView(RoomEvent.ROOM_LIST_CHANGE, onRoomListChange);
			
		}
	}
}