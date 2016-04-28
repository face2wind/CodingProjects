package modules.game.view
{
	import face2wind.uiComponents.ResBitmap;
	import face2wind.view.BaseSprite;
	
	import socketCommand.customData.PokerInfo;
	
	/**
	 * 扑克视图
	 * @author face2wind
	 */
	public class PokerView extends BaseSprite
	{
		public function PokerView(_newIsBack:Boolean = false)
		{
			super();
			isBack = _newIsBack;
		}
		
		/**
		 * 扑克信息
		 */
		private var _pvo:PokerInfo;
		/**
		 * 扑克信息
		 */
		public function set pvo(value:PokerInfo):void
		{
			if(_pvo == value)
				return;
			
			_pvo = value;
			propertyChange();
		}
		public function get pvo():PokerInfo
		{
			return _pvo;
		}
		
		/**
		 * 是否用背面
		 */
		private var _isBack:Boolean;
		/**
		 * 是否用背面
		 */
		public function set isBack(value:Boolean):void
		{
			if(_isBack == value)
				return;
			
			_isBack = value;
			propertyChange();
		}
		
		private var _pokerBm:ResBitmap;
		
		/**
		 * 此函数是视图的内容初始化函数<br/>对父类的覆盖 
		 * 
		 */		
		protected override function createChildren():void
		{
			super.createChildren();
			_pokerBm = new ResBitmap();
			_pokerBm.scaleToWidth(130);
			addChild(_pokerBm);
		}
		
		protected override function update():void
		{
			super.update();
			if(_isBack)
				_pokerBm.source = "poker_back";
			else if(_pokerBm && _pvo)
				_pokerBm.source = "poker_"+_pvo.value+"_"+_pvo.color;
		}
		
		
		/**
		 * 被加到显示列表时执行 
		 */		
		public override function onShowHandler():void
		{
			
		}
		
		/**
		 * 从显示列表移除时执行 
		 */		
		public override function onHideHandler():void
		{
			
		}
	}
}