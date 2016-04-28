package modules.login.event
{
	public class LoginEvent
	{
		public function LoginEvent()
		{
		}
		public static const REQUEST_LOGIN:String = "LoginEvent_REQUEST_LOGIN";
		
		public static const CREATE_ACCOUNT:String = "LoginEvent_CREATE_ACCOUNT";
		
		public static var LOGIN_SUCCESS:String = "LoginEvent_LOGIN_SUCCESS";
	}
}