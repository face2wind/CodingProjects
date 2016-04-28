package modules.login.view
{
	import com.greensock.TweenLite;
	
	import face2wind.enum.LayerEnum;
	import face2wind.event.ParamEvent;
	import face2wind.manager.LayerManager;
	import face2wind.uiComponents.Button;
	import face2wind.uiComponents.CustomTextfield;
	import face2wind.uiComponents.TextInput;
	import face2wind.util.ColorUtil;
	import face2wind.view.BaseSprite;
	
	import flash.events.MouseEvent;
	import flash.text.TextFormatAlign;
	
	import modules.login.event.LoginEvent;

	/**
	 * 登录视图 
	 * @author face2wind
	 */	
	public class LoginView extends BaseSprite
	{
		private var loginBtn:Button;
		private var loginName:TextInput;
		private var loginPasswd:TextInput;
		private var createAccTxt:CustomTextfield;
		
		private var createAccView:CreateAccountView;
		
		public function LoginView()
		{
			super();
		}
		
		protected override function createChildren():void
		{
			super.createChildren();
			
			var nameTxt:CustomTextfield = new CustomTextfield();
			nameTxt.text = "帐号名：";
			nameTxt.align = TextFormatAlign.RIGHT;
			nameTxt.width = 50;
			nameTxt.height = 24;
			nameTxt.move(0,20);
			addChild(nameTxt);
			
			loginName = new TextInput();
			loginName.resize(100, 24);
			loginName.move(nameTxt.x+nameTxt.width, nameTxt.y);
			addChild(loginName);
			
			var passTxt:CustomTextfield = new CustomTextfield();
			passTxt.text = "密码：";
			passTxt.align = TextFormatAlign.RIGHT;
			passTxt.width = 50;
			passTxt.height = 24;
			passTxt.move(0,50);
			addChild(passTxt);
			
			loginPasswd = new TextInput();
			loginPasswd.resize(100, 24);
			loginPasswd.move(passTxt.x+passTxt.width, passTxt.y);
			addChild(loginPasswd);
			
			loginBtn = new Button();
			loginBtn.label = "登录";
			loginBtn.move(30,loginPasswd.y+40);
			addChild(loginBtn);
			
			createAccTxt = new CustomTextfield();
			createAccTxt.htmlText = "<font color='"+ColorUtil.GREEN_S+"'>没有帐号？<u><a href='event:1'>点击这里创建</a></u></font>";
			createAccTxt.move(0,loginBtn.y+40);
			createAccTxt.mouseEnabled = true;
			addChild(createAccTxt);
			
			createAccView = new CreateAccountView();
			addChild(createAccView);
		}
		
		public function show():void
		{
			var layer:BaseSprite = LayerManager.getInstance().getLayer(LayerEnum.LOADING_LAYER);
			layer.addChild(this);
		}
		
		public function hide():void
		{
			var layer:BaseSprite = LayerManager.getInstance().getLayer(LayerEnum.LOADING_LAYER);
			if(layer.contains(this))
				layer.removeChild(this);
		}
		
		protected function onLoginHandler(event:MouseEvent):void
		{
			eventManager.dispatchToController(new ParamEvent(LoginEvent.REQUEST_LOGIN,
				{loginName:loginName.text, loginPassword:loginPasswd.text}));
		}
		
		protected function onShowCreateAccountHandler(event:MouseEvent):void
		{
			TweenLite.to(createAccView, 1, {y:0});
		}
		
		public override function onShowHandler():void
		{
			super.onShowHandler();
			
			loginBtn.addEventListener(MouseEvent.CLICK, onLoginHandler);
			createAccTxt.addEventListener(MouseEvent.CLICK, onShowCreateAccountHandler);
		}
		
		public override function onHideHandler():void
		{
			super.onHideHandler();
			
			loginBtn.removeEventListener(MouseEvent.CLICK, onLoginHandler);
			createAccTxt.removeEventListener(MouseEvent.CLICK, onShowCreateAccountHandler);
		}
	}
}