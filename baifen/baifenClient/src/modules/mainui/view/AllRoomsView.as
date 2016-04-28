package modules.mainui.view
{
	import face2wind.customUIComponents.ItemList;
	import face2wind.customUIComponents.Message;
	import face2wind.event.ParamEvent;
	import face2wind.manager.TimerManager;
	import face2wind.uiComponents.Button;
	import face2wind.uiComponents.Page;
	import face2wind.view.BaseSprite;
	
	import flash.events.MouseEvent;
	
	import modules.mainui.event.RoomEvent;
	import modules.mainui.view.item.RoomListItemRenderer;
	
	/**
	 * 大厅视图（显示所有房间）
	 * @author face2wind
	 */
	public class AllRoomsView extends BaseSprite
	{
		private var allRooms:ItemList;
		private var createRoomBtn:Button;
		private var _page:Page;
		
		private var _roomView:RoomView;
		private var exitBtn:Button;
		
		public function AllRoomsView()
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
			
			_roomView = new RoomView();
			
			graphics.beginFill(0xffff00);
			graphics.drawRoundRect(0,0,300,300,10);
			graphics.endFill();
			
			allRooms = new ItemList();
			allRooms.itemRender = RoomListItemRenderer;
			allRooms.move(100,100);
			allRooms.setActualSize(300,250);
			addChild(allRooms);
			
			createRoomBtn = new Button();
			createRoomBtn.label = "创建房间";
			createRoomBtn.move(20,10);
			addChild(createRoomBtn);
			
			_page = new Page();
			_page.move(130, 355);
			_page.pageChangeCallBack = onPageChangeHandle;
			addChild(_page);
			
			exitBtn = new Button();
			exitBtn.label = "返回";
			exitBtn.move(300,270);
			addChild(exitBtn);
		}
		
		protected function onCreateRoomHandler(event:MouseEvent):void
		{
			eventManager.dispatchToController(new ParamEvent(RoomEvent.REQUEST_CREATE_ROOM));
		}
		
		private function onPageChangeHandle(curPage:int):void
		{
			eventManager.dispatchToController(new ParamEvent(RoomEvent.REQUEST_ROOM_LIST, {page:curPage, handle:onPageListDataHandle}) );
		}
		
		private function onPageListDataHandle(curPage:int, roomList:Array):void
		{
			allRooms.dataProvider = roomList;
		}
		
		private function requestPageData():void
		{
			eventManager.dispatchToController(new ParamEvent(RoomEvent.REQUEST_ROOM_LIST, {page:_page.curPage, handle:onPageListDataHandle}) );
		}
		
		private function onCreateSuccessHandler(e:ParamEvent):void
		{
			addChild(_roomView);
			requestPageData();
			TimerManager.getInstance().removeItem(requestPageData);
		}
		
		private function onExitRoomHandler(e:ParamEvent):void
		{
			if(contains(_roomView))
				removeChild(_roomView);
			TimerManager.getInstance().removeItem(requestPageData);
			TimerManager.getInstance().addItem(1000, requestPageData);
			requestPageData();
		}
		
		protected function onRequestExitRoomHandler(event:MouseEvent):void
		{
			if(parent)
				parent.removeChild(this);
		}
		
		/**
		 * 当前对象被加到显示列表里时触发
		 */		
		public override function onShowHandler():void
		{
			super.onShowHandler();
			
			createRoomBtn.addEventListener(MouseEvent.CLICK, onCreateRoomHandler);
			eventManager.bindToView(RoomEvent.ENTER_ROOM_SUCCESS, onCreateSuccessHandler);
			eventManager.bindToView(RoomEvent.EXIT_ROOM_SUCCESS, onExitRoomHandler);
			exitBtn.addEventListener(MouseEvent.CLICK, onRequestExitRoomHandler);
			TimerManager.getInstance().removeItem(requestPageData);
			TimerManager.getInstance().addItem(1000, requestPageData);
			requestPageData();
		}
		
		/**
		 * 当前对象从显示列表里移除时触发 
		 */		
		public override function onHideHandler():void
		{
			super.onHideHandler();
			
			createRoomBtn.removeEventListener(MouseEvent.CLICK, onCreateRoomHandler);
			eventManager.unbindToView(RoomEvent.ENTER_ROOM_SUCCESS, onCreateSuccessHandler);
			eventManager.unbindToView(RoomEvent.EXIT_ROOM_SUCCESS, onExitRoomHandler);
			exitBtn.addEventListener(MouseEvent.CLICK, onRequestExitRoomHandler);
			TimerManager.getInstance().removeItem(requestPageData);
		}
	}
}