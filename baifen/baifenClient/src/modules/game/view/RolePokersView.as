package modules.game.view
{
	import com.greensock.TweenLite;
	
	import face2wind.event.ParamEvent;
	import face2wind.lib.ObjectPool;
	import face2wind.manager.TimerManager;
	import face2wind.uiComponents.Button;
	import face2wind.view.BaseSprite;
	
	import flash.events.MouseEvent;
	
	import modules.game.event.GameEvent;
	
	import socketCommand.customData.PokerInfo;
	
	/**
	 * 主角扑克牌列表视图
	 * @author face2wind
	 */
	public class RolePokersView extends BaseSprite
	{
		public function RolePokersView()
		{
			super();
		}
		
		private var _pokerList:Array;
		private var _pokerViewList:Array;
		private var curDelayShowIndex:int;
		private var _curSelectedPokerList:Array;
		private var sendCardBtn:Button;
		public function set pokerList(value:Array):void
		{
			if(_pokerList == value)
				return;
			_pokerList = value;
			propertyChange();
		}

		
		/**
		 * 此函数是视图的内容初始化函数<br/>对父类的覆盖 
		 * 
		 */		
		protected override function createChildren():void
		{
			super.createChildren();
			_pokerList = [];
			_pokerViewList = [];
			_curSelectedPokerList = [];
			
			sendCardBtn = new Button();
			sendCardBtn.label = "出牌";
			sendCardBtn.move(50, -30);
			addChild(sendCardBtn);
		}
		
		public function set isMyTurn(value:Boolean):void
		{
			sendCardBtn.visible = value;
		}
		
		protected override function update():void
		{
			super.update();
			removeAllChildren(true);
			for (var i:int = 0; i < _pokerList.length; i++) 
			{
				var poker:PokerInfo = _pokerList[i];
				var pokerView:PokerView = ObjectPool.getObject(PokerView);
				pokerView.pvo = poker;
				pokerView.name = "normal";
				pokerView.move(i*40,0);
//				pokerView.visible = false;
				pokerView.addEventListener(MouseEvent.CLICK, onPokerClickedHandle);
				addChild(pokerView);
				_pokerViewList.push(pokerView);
			}
			addChild(sendCardBtn);
			_curSelectedPokerList = [];
//			curDelayShowIndex = 0;
//			TimerManager.getInstance().removeItem(delayShowPoker);
//			TimerManager.getInstance().addItem(100, delayShowPoker);
		}
		
		private function delayShowPoker():void
		{
			if(curDelayShowIndex >= _pokerViewList.length){
				TimerManager.getInstance().removeItem(delayShowPoker);
				return;
			}
			var pokerView:PokerView = _pokerViewList[curDelayShowIndex];
			pokerView.visible = true;
			curDelayShowIndex++;
		}
		
		protected function onPokerClickedHandle(event:MouseEvent):void
		{
			var pokerView:PokerView = event.currentTarget as PokerView;
			if(null == pokerView)
				return;
			
			TweenLite.killTweensOf(pokerView);
			if(pokerView.name == "normal"){
				pokerView.name = "selected";
				TweenLite.to(pokerView, 0.2, {y:-50});
				_curSelectedPokerList.push(pokerView.pvo);
			}else{
				pokerView.name = "normal";
				TweenLite.to(pokerView, 0.2, {y:0});
				_curSelectedPokerList.splice(_curSelectedPokerList.indexOf(pokerView.pvo),1);
			}
		}
		
		protected function onSendCardHandle(event:MouseEvent):void
		{
			eventManager.dispatchToController(new ParamEvent(GameEvent.REQUEST_SEND_CARDS, {pokerList:_curSelectedPokerList}));
		}
		
		/**
		 * 被加到显示列表时执行 
		 */		
		public override function onShowHandler():void
		{
			for (var i:int = 0; i < _pokerViewList.length; i++) 
			{
				var pokerView:PokerView = _pokerViewList[i];
				pokerView.addEventListener(MouseEvent.CLICK, onPokerClickedHandle);
			}
			sendCardBtn.addEventListener(MouseEvent.CLICK, onSendCardHandle);
		}
		
		/**
		 * 从显示列表移除时执行 
		 */		
		public override function onHideHandler():void
		{
			for (var i:int = 0; i < _pokerViewList.length; i++) 
			{
				var pokerView:PokerView = _pokerViewList[i];
				pokerView.removeEventListener(MouseEvent.CLICK, onPokerClickedHandle);
			}
			sendCardBtn.removeEventListener(MouseEvent.CLICK, onSendCardHandle);
		}
	}
}