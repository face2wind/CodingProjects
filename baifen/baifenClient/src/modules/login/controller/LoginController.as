package modules.login.controller
{
	import cache.PlayerCache;
	import cache.item.PlayerVo;
	
	import common.ErrorMessage;
	
	import face2wind.base.Controller;
	import face2wind.customUIComponents.Message;
	import face2wind.event.ParamEvent;
	import face2wind.manager.TimerManager;
	
	import modules.login.event.LoginEvent;
	import modules.login.view.LoginView;
	
	import socketCommand.c2s.cs100.CS10000;
	import socketCommand.c2s.cs100.CS10002;
	import socketCommand.c2s.cs100.CS10003;
	import socketCommand.s2c.sc100.SC10000;
	import socketCommand.s2c.sc100.SC10001;
	import socketCommand.s2c.sc100.SC10002;
	import socketCommand.s2c.sc100.SC10003;
	
	/**
	 * 登录控制器
	 * @author face2wind
	 */
	public class LoginController extends Controller
	{
		public function LoginController()
		{
			if(instance)
				throw new Error("LoginController is singleton class and allready exists!");
			instance = this;
			
			addCmdListener(10000, on10000handler);
			addCmdListener(10001, on10001handler);
			addCmdListener(10002, on10002handler);
			addCmdListener(10003, on10003handler);
			
			eventManager.bindToController(LoginEvent.REQUEST_LOGIN, onRequestLoginHandle);
			eventManager.bindToController(LoginEvent.CREATE_ACCOUNT, onRequestCreateAccountHandle);
			
			loginView = new LoginView();
			loginView.show();
		}
		
		/**
		 * 单例
		 */
		private static var instance:LoginController;
		/**
		 * 获取单例
		 */
		public static function getInstance():LoginController
		{
			if(!instance)
				instance = new LoginController();
			return instance;
		}
		
		/**
		 * 登录视图 
		 */		
		private var loginView:LoginView;
		
		/**
		 * 发出创建帐号请求 
		 */	
		private function onRequestCreateAccountHandle(e:ParamEvent):void
		{
			var cmd:CS10002 = new CS10002();
			cmd.loginName = e.param.loginName;
			cmd.password = e.param.loginPassword;
			sendMessage(10002, cmd);
		}
		
		/**
		 * 发出登录请求 
		 */	
		private function onRequestLoginHandle(e:ParamEvent):void
		{
			var cmd:CS10003 = new CS10003();
			cmd.loginName = e.param.loginName;
			cmd.password = e.param.loginPassword;
			sendMessage(10003, cmd);
		}
		
		private function heartBeatTimerHandler():void
		{
			var cmd:CS10000 = new CS10000();
			cmd.timeStamp = (new Date()).time/1000;
			Message.show(">>>>>>>>> send time : "+cmd.timeStamp);
			sendMessage(10000, cmd);
		}
		
		/**
		 * 心跳包 
		 * @param scmd
		 */		
		private function on10000handler(scmd:SC10000):void
		{
			Message.show("(((((((((((((((((( server time : "+scmd.timeStamp);
		}
		
		/**
		 * 帐号查询
		 * @param scmd
		 */		
		private function on10001handler(scmd:SC10001):void
		{
			if(scmd.accountExist)
				Message.show("帐号已存在！");
			else
				Message.show("帐号不存在！");
		}
		
		/**
		 * 创建帐号返回 
		 * @param scmd
		 */		
		private function on10002handler(scmd:SC10002):void
		{
			Message.show(ErrorMessage.findMsgWithCode(scmd.errCode));
		}
		
		/**
		 * 登录帐号返回 
		 * @param scmd
		 */		
		private function on10003handler(scmd:SC10003):void
		{
			Message.show(ErrorMessage.findMsgWithCode(scmd.errCode));
			
			if(10002 == scmd.errCode){
				loginView.hide();
				eventManager.dispatchToController(new ParamEvent(LoginEvent.LOGIN_SUCCESS));
				
				var role:PlayerVo = new PlayerVo();
				role.id = scmd.id;
				role.name = scmd.name;
				PlayerCache.getInstance().initRole(role);
//				TimerManager.getInstance().removeItem(heartBeatTimerHandler);
//				TimerManager.getInstance().addItem(10000, heartBeatTimerHandler);
			}
		}
	}
}