package modules.mainui.event
{
	import face2wind.event.ParamEvent;

	/**
	 * 房间相关事件
	 * @author face2wind
	 */
	public class RoomEvent
	{
		public function RoomEvent()
		{
		}
		public static const REQUEST_ROOM_LIST:String = "RoomEvent_REQUEST_ROOM_LIST";
		
		public static const REQUEST_CREATE_ROOM:String = "RoomEvent_REQUEST_CREATE_ROOM";
		
		public static const ENTER_ROOM_SUCCESS:String = "RoomEvent_ENTER_ROOM_SUCCESS";
		
		public static const ROOM_LIST_CHANGE:String = "RoomEvent_ROOM_LIST_CHANGE";
		
		public static const REQUEST_JOIN_ROOM:String = "RoomEvent_REQUEST_JOIN_ROOM";
		
		public static const REQUEST_EXIT_ROOM:String = "RoomEvent_REQUEST_EXIT_ROOM";
		
		public static const EXIT_ROOM_SUCCESS:String = "RoomEvent_EXIT_ROOM_SUCCESS";
	}
}