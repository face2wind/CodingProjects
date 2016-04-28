package socketCommand.customData
{
	/**
	 * 房间信息<br/>
	 * ( 此文件由工具生成，勿手动修改)
	 * @author face2wind
	 */
	public class RoomInfo
	{
		public function RoomInfo()
		{
		}

		/**
		 * 房间号
		 */
		public var roomID:int ;
		/**
		 * 加锁类型（0无锁，1需要密码，2一律不准进入）
		 */
		public var lockType:int ;
		/**
		 * 玩家列表
		 */
		public var playerList:Array = [RoomInfo_playerList] ;
	}
}