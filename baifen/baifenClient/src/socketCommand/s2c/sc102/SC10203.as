package socketCommand.s2c.sc102
{
	import socketCommand.customData.*

	/**
	 * 出牌<br/>
	 * ( 此文件由工具生成，勿手动修改)
	 * @author face2wind
	 */
	public class SC10203
	{
		public function SC10203()
		{
		}
		/**
		 * 出牌的玩家下标
		 */
		public var curTurnIndex:int ;
		/**
		 * 下一个出牌的玩家下标
		 */
		public var nextTurnIndex:int ;
		/**
		 * 出牌列表
		 */
		public var pokerList:Array = [PokerInfo] ;
	}
}