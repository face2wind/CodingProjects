package modules.mainui.controller
{
	import face2wind.base.Controller;
	import face2wind.event.ParamEvent;
	
	import modules.game.event.GameEvent;
	import modules.login.event.LoginEvent;
	import modules.mainui.view.MainUIView;
	
	/**
	 * 主界面控制器
	 * @author face2wind
	 */
	public class MainUIController extends Controller
	{
		public function MainUIController()
		{
			if(instance)
				throw new Error("MainUIController is singleton class and allready exists!");
			instance = this;
			
			mainUIView = new MainUIView();
			
			eventManager.bindToController(LoginEvent.LOGIN_SUCCESS, onLoginSuccessHandle);
			eventManager.bindToController(GameEvent.GAME_STATE_CHANGE, onGameStateChangeHandler);
		}
		
		/**
		 * 单例
		 */
		private static var instance:MainUIController;
		/**
		 * 获取单例
		 */
		public static function getInstance():MainUIController
		{
			if(!instance)
				instance = new MainUIController();
			
			return instance;
		}
		
		private var mainUIView:MainUIView;
		
		private function onLoginSuccessHandle(e:ParamEvent):void
		{
			mainUIView.show();
		}
		
		private function onGameStateChangeHandler(e:ParamEvent):void
		{
			if(0 == e.param.state)
				mainUIView.show();
			else
				mainUIView.hide()
		}
	}
}