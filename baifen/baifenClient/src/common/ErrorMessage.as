package common
{
	import flash.utils.Dictionary;
	
	/**
	 * 错误码管理器<br/>
	 * （为了简便，直接把错误码放代码里，实际项目中需要提取到xml里，跟着项目配置一起序列化打包然后读取）
	 * @author face2wind
	 */
	public class ErrorMessage
	{
		public function ErrorMessage()
		{
		}
		
		private static var _errcodes:Dictionary;
		/**
		 * 错误码容器 
		 */
		private static function get errcodes():Dictionary
		{
			if(null == _errcodes){
				_errcodes = new Dictionary();
				_errcodes[0] = "成功";
				_errcodes[1] = "失败";
				
				_errcodes[10000] = "创建帐号成功";
				_errcodes[10001] = "帐号已被注册过";
				_errcodes[10002] = "登录成功";
				_errcodes[10003] = "该帐号不存在";
				_errcodes[10004] = "密码不正确";
				
				_errcodes[10100] = "成功进入房间";
				_errcodes[10101] = "此位置有人";
				_errcodes[10102] = "密码错误";
				_errcodes[10103] = "没有这个座位";
				_errcodes[10104] = "没有这个房间";
				_errcodes[10105] = "创建房间成功";
				_errcodes[10106] = "加锁类型不可识别";
				_errcodes[10107] = "密码过长";
				_errcodes[10108] = "你已经在一个房间里";
				_errcodes[10109] = "你退出了房间";
				_errcodes[10110] = "正在游戏中不能退出";
				
				_errcodes[10200] = "游戏成功开始";
				_errcodes[10201] = "你不在房间内";
				_errcodes[10202] = "你不是房主";
				_errcodes[10203] = "游戏还未结束";
			}
			return _errcodes;
		}
		
		/**
		 * 获取错误码对应的描述 
		 * @param errCode
		 * @return 
		 */		
		public static function findMsgWithCode(errCode:int):String
		{
			if(null != errcodes[errCode])
				return errcodes[errCode];
			else
				return "unknow error code : "+errCode;
		}
	}
}