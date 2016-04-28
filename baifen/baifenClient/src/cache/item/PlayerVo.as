package cache.item
{
	import face2wind.event.PropertyChangeEvent;
	
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	
	/**
	 * 玩家信息
	 * @author face2wind
	 */
	public class PlayerVo extends EventDispatcher
	{
		public function PlayerVo()
		{
		}
		public var id:int;
		
		/**
		 * 玩家名字
		 */
		private var _name:String;
		/**
		 * 玩家名字
		 */
		public function get name():String
		{
			return _name;
		}
		/**
		 * 玩家名字
		 */
		public function set name(value:String):void
		{
			if(_name == value)
				return;
			
			var _oldValue:* = _name;
			_name = value;
			dispatchEvent(PropertyChangeEvent.createUpdateEvent("name", _oldValue, _name));
		}
	}
}