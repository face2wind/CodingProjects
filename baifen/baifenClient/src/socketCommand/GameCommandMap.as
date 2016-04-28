package socketCommand
{
	import face2wind.net.CommandMap;
	import face2wind.net.item.SocketDataType;

	import socketCommand.customData.*;
	import socketCommand.s2c.sc100.*;
	import socketCommand.s2c.sc101.*;
	import socketCommand.s2c.sc102.*;

	/**
	 * 协议字典类<br/>
	 * ( 此文件由工具生成，勿手动修改)
	 * @author face2wind
	 */
	public class GameCommandMap extends CommandMap
	{
		public function GameCommandMap()
		{
			super();
		}

		/**
		 *  初始化S2C的协议类对象列表
		 */
		protected override function initScmdClassDic():void
		{
			_scmdClassDic["SC10000"] = SC10000;
			_scmdClassDic["SC10001"] = SC10001;
			_scmdClassDic["SC10002"] = SC10002;
			_scmdClassDic["SC10003"] = SC10003;
			_scmdClassDic["SC10100"] = SC10100;
			_scmdClassDic["RoomInfo"] = RoomInfo;
			_scmdClassDic["SC10101"] = SC10101;
			_scmdClassDic["SC10102"] = SC10102;
			_scmdClassDic["SC10103"] = SC10103;
			_scmdClassDic["SC10104"] = SC10104;
			_scmdClassDic["SC10200"] = SC10200;
			_scmdClassDic["SC10201"] = SC10201;
			_scmdClassDic["SC10202"] = SC10202;
			_scmdClassDic["SC10202_pokerNumList"] = SC10202_pokerNumList;
			_scmdClassDic["PokerInfo"] = PokerInfo;
			_scmdClassDic["SC10203"] = SC10203;
			_scmdClassDic["RoomInfo_playerList"] = RoomInfo_playerList;
		}

		/**
		 * 初始化所有协议属性信息
		 */
		protected override function initCMDAttributes():void
		{
			_cmdAttributes["CS10000"] = [{name:"timeStamp", type:SocketDataType.INT32}];
			_cmdAttributes["CS10001"] = [{name:"loginName", type:SocketDataType.STRING}];
			_cmdAttributes["CS10002"] = [{name:"loginName", type:SocketDataType.STRING},{name:"password", type:SocketDataType.STRING}];
			_cmdAttributes["CS10003"] = [{name:"loginName", type:SocketDataType.STRING},{name:"password", type:SocketDataType.STRING}];
			_cmdAttributes["CS10100"] = [{name:"pageNum", type:SocketDataType.INT8}];
			_cmdAttributes["CS10101"] = [{name:"roomID", type:SocketDataType.INT32},{name:"sitID", type:SocketDataType.INT8},{name:"passwd", type:SocketDataType.STRING}];
			_cmdAttributes["CS10102"] = [{name:"lockType", type:SocketDataType.INT8},{name:"passwd", type:SocketDataType.STRING}];
			_cmdAttributes["CS10103"] = [{name:"roomID", type:SocketDataType.INT32},{name:"sitID", type:SocketDataType.INT8}];
			_cmdAttributes["CS10203"] = [{name:"pokerList", type:SocketDataType.ARRAY, subType:"PokerInfo"}];
			_cmdAttributes["PokerInfo"] = [{name:"color", type:SocketDataType.INT8},{name:"value", type:SocketDataType.INT8}];
			_cmdAttributes["SC10000"] = [{name:"timeStamp", type:SocketDataType.INT32}];
			_cmdAttributes["SC10001"] = [{name:"accountExist", type:SocketDataType.INT8}];
			_cmdAttributes["SC10002"] = [{name:"errCode", type:SocketDataType.INT16}];
			_cmdAttributes["SC10003"] = [{name:"errCode", type:SocketDataType.INT16},{name:"id", type:SocketDataType.INT32},{name:"name", type:SocketDataType.STRING}];
			_cmdAttributes["SC10100"] = [{name:"pageNum", type:SocketDataType.INT8},{name:"maxPageNum", type:SocketDataType.INT8},{name:"roomList", type:SocketDataType.ARRAY, subType:"RoomInfo"}];
			_cmdAttributes["RoomInfo"] = [{name:"roomID", type:SocketDataType.INT32},{name:"lockType", type:SocketDataType.INT8},{name:"playerList", type:SocketDataType.ARRAY, subType:"RoomInfo_playerList"}];
			_cmdAttributes["SC10101"] = [{name:"result", type:SocketDataType.INT16},{name:"roomID", type:SocketDataType.INT32}];
			_cmdAttributes["SC10102"] = [{name:"result", type:SocketDataType.INT16},{name:"roomID", type:SocketDataType.INT32}];
			_cmdAttributes["SC10103"] = [{name:"result", type:SocketDataType.INT16},{name:"sitID", type:SocketDataType.INT8}];
			_cmdAttributes["SC10104"] = [{name:"roomInfo", type:"RoomInfo"}];
			_cmdAttributes["SC10200"] = [{name:"gameState", type:SocketDataType.INT16}];
			_cmdAttributes["SC10201"] = [{name:"result", type:SocketDataType.INT16}];
			_cmdAttributes["SC10202"] = [{name:"roleIndex", type:SocketDataType.INT8},{name:"nextTurnIndex", type:SocketDataType.INT8},{name:"pokerNumList", type:SocketDataType.ARRAY, subType:"SC10202_pokerNumList"},{name:"pokerList", type:SocketDataType.ARRAY, subType:"PokerInfo"}];
			_cmdAttributes["SC10202_pokerNumList"] = [{name:"pokerNum", type:SocketDataType.INT8}];
			_cmdAttributes["PokerInfo"] = [{name:"color", type:SocketDataType.INT8},{name:"value", type:SocketDataType.INT8}];
			_cmdAttributes["SC10203"] = [{name:"curTurnIndex", type:SocketDataType.INT8},{name:"nextTurnIndex", type:SocketDataType.INT8},{name:"pokerList", type:SocketDataType.ARRAY, subType:"PokerInfo"}];
			_cmdAttributes["RoomInfo_playerList"] = [{name:"playerID", type:SocketDataType.INT32},{name:"playerName", type:SocketDataType.STRING},{name:"sitID", type:SocketDataType.INT8},{name:"isOwner", type:SocketDataType.INT8}];
		}
	}
}