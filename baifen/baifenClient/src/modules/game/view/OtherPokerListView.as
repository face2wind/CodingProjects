package modules.game.view
{
	import face2wind.manager.TimerManager;
	import face2wind.view.BaseSprite;
	
	/**
	 * 其他玩家的扑克列表
	 * @author face2wind
	 */
	public class OtherPokerListView extends BaseSprite
	{
		private var isVertical:Boolean;
		public function OtherPokerListView(vertical:Boolean = true)
		{
			super();
			isVertical = vertical;
		}
		
		/**
		 * 扑克牌数量
		 */
		private var _pokerNum:int;
		private var curDelayShowIndex:int;
		/**
		 * 扑克牌数量
		 */
		public function get pokerNum():int
		{
			return _pokerNum;
		}
		/**
		 * 扑克牌数量
		 */
		public function set pokerNum(value:int):void
		{
			if(_pokerNum == value)
				return;
			
			_pokerNum = value;
			propertyChange();
		}
		private var _pokerViewList:Array;
		
		/**
		 * 此函数是视图的内容初始化函数<br/>对父类的覆盖 
		 * 
		 */		
		protected override function createChildren():void
		{
			super.createChildren();
			_pokerViewList = [];
		}
		
		protected override function update():void
		{
			super.update();
			removeAllChildren(true);
			for (var i:int = 0; i < _pokerNum; i++) 
			{
				var targetX:int = 0;
				var targetY:int = 0;
				if(isVertical)
					targetX = i*30;
				else
					targetY = i*30;
				var pokerView:PokerView = new PokerView(true);
				pokerView.move(targetX,targetY);
//				pokerView.visible = false;
				addChild(pokerView);
				_pokerViewList.push(pokerView);
			}
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
	}
}