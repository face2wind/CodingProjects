package modules.mainui.view.item
{
	import face2wind.customUIComponents.Message;
	import face2wind.customUIComponents.item.ItemRenderer;
	import face2wind.event.ParamEvent;
	import face2wind.event.PropertyChangeEvent;
	import face2wind.uiComponents.CustomTextfield;
	
	import flash.events.MouseEvent;
	import flash.text.TextFormatAlign;
	
	import modules.mainui.event.RoomEvent;
	
	import socketCommand.customData.RoomInfo;
	import socketCommand.customData.RoomInfo_playerList;
	
	/**
	 * 房间列表项
	 * @author face2wind
	 */
	public class RoomListItemRenderer extends ItemRenderer
	{
		private var vo:RoomInfo;
		private var roomNum:CustomTextfield;
		public function RoomListItemRenderer()
		{
			super();
		}
		
		/**
		 * 此函数是视图的内容初始化函数<br/>对父类的覆盖 
		 * 
		 */		
		protected override function createChildren():void
		{
			super.createChildren();
			
			buttonMode = true;
			
			graphics.beginFill(0x00ff00);
			graphics.drawRoundRect(0,0,50,50,10);
			graphics.endFill();
			
			 roomNum = new CustomTextfield();
			 roomNum.move(0,10);
			 roomNum.width = 50;
			 roomNum.align = TextFormatAlign.CENTER;
			 addChild(roomNum);
		}
		
		public override function set data(value:Object):void
		{
			if(vo == value)
				return;
//			if(null != vo)
//				vo.removeEventListener(PropertyChangeEvent.PROPERTY_CHANGE, onVoPropertyChange);
			vo = value as RoomInfo;
//			if(null != vo)
//				vo.addEventListener(PropertyChangeEvent.PROPERTY_CHANGE, onVoPropertyChange);
			propertyChange();
		}
		
		private function onVoPropertyChange(e:PropertyChangeEvent):void
		{
			
		}
		
		protected override function update():void
		{
			super.update();
			if(null == vo)
				return;
			roomNum.text = vo.roomID+"号";
			tooltip = "加入"+roomNum.text+"房间";
		}
		
		protected function onClickedHandle(event:MouseEvent):void
		{
			if(null == vo)
				return;
			var emptySit:int = -1;
			var emptySits:Array = [0,1,2,3];
			for (var i:int = 0; i < vo.playerList.length; i++) 
			{
				var list:RoomInfo_playerList = vo.playerList[i];
				emptySits[i] = -1;
			}
			for (var j:int = 0; j < emptySits.length; j++) 
			{
				if(-1 != emptySits[j]){
					emptySit = emptySits[j];
					break;
				}
			}
			
			if(-1 == emptySit)
				Message.show("该房间已经满人");
			else
				eventManager.dispatchToController(new ParamEvent(RoomEvent.REQUEST_JOIN_ROOM, {roomID:vo.roomID,sitID:emptySit}));
		}
		
		/**
		 * 被加到显示列表时执行 
		 */		
		public override function onShowHandler():void
		{
			addEventListener(MouseEvent.CLICK, onClickedHandle);
		}
		
		/**
		 * 从显示列表移除时执行 
		 */		
		public override function onHideHandler():void
		{
			removeEventListener(MouseEvent.CLICK, onClickedHandle);
		}
	}
}