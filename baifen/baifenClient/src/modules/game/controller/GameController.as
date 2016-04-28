package modules.game.controller
{
	import common.ErrorMessage;
	
	import face2wind.base.Controller;
	import face2wind.customUIComponents.Message;
	import face2wind.event.ParamEvent;
	
	import modules.game.event.GameEvent;
	import modules.game.model.GameManager;
	import modules.game.view.GameView;
	
	import socketCommand.c2s.cs102.CS10203;
	import socketCommand.s2c.sc102.SC10200;
	import socketCommand.s2c.sc102.SC10201;
	import socketCommand.s2c.sc102.SC10202;
	import socketCommand.s2c.sc102.SC10203;
	
	/**
	 * 游戏控制器
	 * @author face2wind
	 */
	public class GameController extends Controller
	{
		public function GameController()
		{
			if(instance)
				throw new Error("GameController is singleton class and allready exists!");
			instance = this;
			manager = GameManager.getInstance();
			
			eventManager.bindToController(GameEvent.REQUEST_START_GAME, onRequestGameStartHandler);
			eventManager.bindToController(GameEvent.GAME_STATE_CHANGE, onGameStateChangeHandler);
			eventManager.bindToController(GameEvent.REQUEST_SEND_CARDS,onRequestSendCardsHandler);
			
			addCmdListener(10200, on10200handle);
			addCmdListener(10201, on10201handle);
			addCmdListener(10202, on10202handle);
			addCmdListener(10203, on10203handle);
		}
		
		/**
		 * 单例
		 */
		private static var instance:GameController;
		/**
		 * 获取单例
		 */
		public static function getInstance():GameController
		{
			if(!instance)
				instance = new GameController();
			return instance;
		}
		
		private var _gameView:GameView;
		private var manager:GameManager;
		public function get gameView():GameView
		{
			if(null == _gameView)
				_gameView = new GameView();
			return _gameView;
		}

		
		private function onRequestGameStartHandler(e:ParamEvent):void
		{
			sendMessage(10201);
		}
		
		private function onGameStateChangeHandler(e:ParamEvent):void
		{
			if(0 == e.param.state)
				gameView.hide()
			else
				gameView.show();
		}
		
		private function onRequestSendCardsHandler(e:ParamEvent):void
		{
			if(null == e.param.pokerList || 0 == e.param.pokerList.length){
				Message.show("你还没选中要出的牌");
				return;
			}
			var cmd:CS10203 = new CS10203();
			cmd.pokerList = e.param.pokerList;
			sendMessage(10203,cmd);
		}
		
		// 协议接收 ========================================================
		
		private function on10200handle(sc:SC10200):void
		{
			eventManager.dispatchToController(new ParamEvent(GameEvent.GAME_STATE_CHANGE, {state:sc.gameState}));
		}
		
		private function on10201handle(sc:SC10201):void
		{
			Message.show( ErrorMessage.findMsgWithCode(sc.result));
		}
		
		private function on10202handle(sc:SC10202):void
		{
			manager.curRoleIndex = sc.roleIndex;
			manager.nextTurnPlayerIndex = sc.nextTurnIndex;
			manager.allPlayerPokerNums = sc.pokerNumList;
			manager.curRolePokerList = sc.pokerList;
			gameView.clearTablePokers();
		}
		
		private function on10203handle(sc:SC10203):void
		{
			manager.outCards(sc.curTurnIndex, sc.pokerList);
			manager.nextTurnPlayerIndex = sc.nextTurnIndex;
		}
	}
}