package modules.game.model
{
	import face2wind.event.ParamEvent;
	import face2wind.event.PropertyChangeEvent;
	import face2wind.manager.EventManager;
	
	import flash.events.EventDispatcher;
	
	import modules.game.event.GameEvent;
	
	import socketCommand.customData.PokerInfo;

	/**
	 * 游戏数据管理器
	 * @author face2wind
	 */
	public class GameManager extends EventDispatcher
	{
		public function GameManager()
		{
			if(instance)
				throw new Error("GameManager is singleton class and allready exists!");
			instance = this;
		}
		
		/**
		 * 单例
		 */
		private static var instance:GameManager;
		/**
		 * 获取单例
		 */
		public static function getInstance():GameManager
		{
			if(!instance)
				instance = new GameManager();
			
			return instance;
		}
		/**
		 * 下一个出牌的玩家下标
		 */
		private var _nextTurnPlayerIndex:int = -1;
		/**
		 * 下一个出牌的玩家下标
		 */
		public function get nextTurnPlayerIndex():int
		{
			return _nextTurnPlayerIndex;
		}
		/**
		 * 下一个出牌的玩家下标
		 */
		public function set nextTurnPlayerIndex(value:int):void
		{
			if(_nextTurnPlayerIndex == value)
				return;
			
			var _oldValue:* = _nextTurnPlayerIndex;
			_nextTurnPlayerIndex = value;
			dispatchEvent(PropertyChangeEvent.createUpdateEvent("nextTurnPlayerIndex", _oldValue, _nextTurnPlayerIndex));
		}
		
		/**
		 * 当前主角手上的牌列表
		 */
		private var _curRolePokerList:Array;
		/**
		 * 当前主角手上的牌列表
		 */
		public function get curRolePokerList():Array
		{
			return _curRolePokerList;
		}
		/**
		 * 当前主角手上的牌列表
		 */
		public function set curRolePokerList(value:Array):void
		{
			if(_curRolePokerList == value)
				return;
			
			var _oldValue:* = _curRolePokerList;
			_curRolePokerList = value;
			dispatchEvent(PropertyChangeEvent.createUpdateEvent("curRolePokerList", _oldValue, _curRolePokerList));
		}
		
		/**
		 * 所有玩家手上牌个数
		 */
		private var _allPlayerPokerNums:Array;
		/**
		 * 所有玩家手上牌个数
		 */
		public function get allPlayerPokerNums():Array
		{
			return _allPlayerPokerNums;
		}
		/**
		 * 所有玩家手上牌个数
		 */
		public function set allPlayerPokerNums(value:Array):void
		{
			if(_allPlayerPokerNums == value)
				return;
			
			var _oldValue:* = _allPlayerPokerNums;
			_allPlayerPokerNums = value;
			dispatchEvent(PropertyChangeEvent.createUpdateEvent("allPlayerPokerNums", _oldValue, _allPlayerPokerNums));
		}
		
		/**
		 * 主角在房间里的下标
		 */
		private var _curRoleIndex:int = -1;
		/**
		 * 主角在房间里的下标
		 */
		public function get curRoleIndex():int
		{
			return _curRoleIndex;
		}
		/**
		 * 主角在房间里的下标
		 */
		public function set curRoleIndex(value:int):void
		{
			if(_curRoleIndex == value)
				return;
			
			var _oldValue:* = _curRoleIndex;
			_curRoleIndex = value;
			dispatchEvent(PropertyChangeEvent.createUpdateEvent("curRoleIndex", _oldValue, _curRoleIndex));
		}
		
		/**
		 *  后端通知出牌
		 * @param curTurnIndex
		 * @param pokerList
		 */		
		public function outCards(outerIndex:int, pokerList:Array):void
		{
			if(outerIndex == curRoleIndex){ // 主角出牌
				var newArr:Array = [];
				for (var i:int = 0; i < _curRolePokerList.length; i++) 
				{
					var pinfo:PokerInfo = _curRolePokerList[i] as PokerInfo;
					var found:Boolean = false;
					for (var j:int = 0; j < pokerList.length; j++) 
					{
						if(pokerList[j].color == pinfo.color &&
							pokerList[j].value == pinfo.value){
							found = true;
							break;
						}
					}
					if(!found)
						newArr.push(pinfo);
				}
				curRolePokerList = newArr;
				allPlayerPokerNums[outerIndex] = newArr.length;
			}else{
				if(null == allPlayerPokerNums[outerIndex])
					return;
				allPlayerPokerNums[outerIndex].pokerNum -= pokerList.length;
				dispatchEvent(PropertyChangeEvent.createUpdateEvent("allPlayerPokerNums", null, _allPlayerPokerNums));
			}
			EventManager.getInstance().dispatchToView(new ParamEvent(GameEvent.SHOW_SEND_CARDS_EFFECT,{index:outerIndex, pokerList:pokerList}));
		}
	}// class end
}