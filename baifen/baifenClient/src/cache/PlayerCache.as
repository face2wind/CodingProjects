package cache
{
	import cache.item.PlayerVo;
	
	import flash.utils.Dictionary;

	/**
	 * 玩家信息缓存
	 * @author face2wind
	 */
	public class PlayerCache
	{
		public function PlayerCache()
		{
			if(instance)
				throw new Error("PlayerCache is singleton class and allready exists!");
			instance = this;
			
			_playerDic = new Dictionary();
		}
		
		/**
		 * 单例
		 */
		private static var instance:PlayerCache;
		/**
		 * 获取单例
		 */
		public static function getInstance():PlayerCache
		{
			if(!instance)
				instance = new PlayerCache();
			
			return instance;
		}
		
		private var _playerDic:Dictionary;
		
		private var _roleVo:PlayerVo;
		
		public function initRole(role:PlayerVo):void
		{
			if(null == role)
				return;
			_roleVo = role;
			_playerDic[_roleVo.id] = _roleVo;
		}
		
		public function get roleVo():PlayerVo
		{
			return _roleVo;
		}
	}
}