package modules.game.event
{
	import face2wind.event.ParamEvent;

	/**
	 * 游戏事件
	 * @author face2wind
	 */
	public class GameEvent
	{
		public function GameEvent()
		{
		}
		public static const REQUEST_START_GAME:String = "GameEvent_REQUEST_START_GAME";
		
		public static var GAME_STATE_CHANGE:String = "GameEvent_GAME_STATE_CHANGE";
		
		public static var REQUEST_SEND_CARDS:String = "GameEvent_REQUEST_SEND_CARDS";
		
		public static var SHOW_SEND_CARDS_EFFECT:String = "GameEvent_SHOW_SEND_CARDS_EFFECT";
	}
}