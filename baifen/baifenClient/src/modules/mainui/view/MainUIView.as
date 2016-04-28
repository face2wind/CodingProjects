package modules.mainui.view
{
	import face2wind.customUIComponents.Message;
	import face2wind.enum.LayerEnum;
	import face2wind.manager.LayerManager;
	import face2wind.uiComponents.Button;
	import face2wind.uiComponents.enum.ButtonType;
	import face2wind.view.BaseSprite;
	
	import flash.events.MouseEvent;
	
	/**
	 * 主界面
	 * @author face2wind
	 */
	public class MainUIView extends BaseSprite
	{
		private var singleGameBtn:Button;
		private var multiGameBtn:Button;
		
		private var allRView:AllRoomsView;
		
		public function MainUIView()
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
			
			singleGameBtn = new Button();
			singleGameBtn.label = "单机游戏";
			singleGameBtn.move(100,30);
			singleGameBtn.resize(100,30);
			addChild(singleGameBtn);
			
			multiGameBtn = new Button();
			multiGameBtn.label = "联网游戏";
			multiGameBtn.move(singleGameBtn.x, multiGameBtn.y+60);
			multiGameBtn.resize(100,30);
			addChild(multiGameBtn);
			
			allRView = new AllRoomsView();
		}
		
		public function show():void
		{
			var layer:BaseSprite = LayerManager.getInstance().getLayer(LayerEnum.LOADING_LAYER);
			layer.addChild(this);
		}
		
		public function hide():void
		{
			var layer:BaseSprite = LayerManager.getInstance().getLayer(LayerEnum.LOADING_LAYER);
			if(layer.contains(this))
				layer.removeChild(this);
		}
		
		private function onSingleGameHandler(e:MouseEvent):void
		{
			Message.show("功能暂不开放");
		}
		
		private function onMultiGameHandler(e:MouseEvent):void
		{
			addChild(allRView);
		}
		
		/**
		 * 当前对象被加到显示列表里时触发
		 */		
		public override function onShowHandler():void
		{
			super.onShowHandler();
			
			singleGameBtn.addEventListener(MouseEvent.CLICK, onSingleGameHandler);
			multiGameBtn.addEventListener(MouseEvent.CLICK, onMultiGameHandler);
		}
		
		/**
		 * 当前对象从显示列表里移除时触发 
		 */		
		public override function onHideHandler():void
		{
			super.onHideHandler();
			
			singleGameBtn.removeEventListener(MouseEvent.CLICK, onSingleGameHandler);
			multiGameBtn.removeEventListener(MouseEvent.CLICK, onMultiGameHandler);
		}
	}
}