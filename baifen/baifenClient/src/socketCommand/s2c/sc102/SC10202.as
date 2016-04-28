package socketCommand.s2c.sc102
{
	import socketCommand.customData.*

	/**
	 * 发牌、更新当前牌面信息<br/>
	 * ( 此文件由工具生成，勿手动修改)
	 * @author face2wind
	 */
	public class SC10202
	{
		public function SC10202()
		{
		}
		/**
		 * 自己的下标
		 */
		public var roleIndex:int ;
		/**
		 * 下一个出牌的玩家下标
		 */
		public var nextTurnIndex:int ;
		/**
		 * 所有人手上的牌个数
		 */
		public var pokerNumList:Array = [SC10202_pokerNumList] ;
		/**
		 * 自己手上的牌
		 */
		public var pokerList:Array = [PokerInfo] ;
	}
}