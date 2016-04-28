package modules.login.view
{
	import com.greensock.TweenLite;
	
	import face2wind.customUIComponents.Message;
	import face2wind.event.ParamEvent;
	import face2wind.uiComponents.Button;
	import face2wind.uiComponents.CustomTextfield;
	import face2wind.uiComponents.TextInput;
	import face2wind.util.ColorUtil;
	import face2wind.view.BaseSprite;
	
	import flash.events.MouseEvent;
	import flash.text.TextFormatAlign;
	
	import modules.login.event.LoginEvent;
	
	/**
	 * 创建角色界面
	 * @author face2wind
	 */
	public class CreateAccountView extends BaseSprite
	{
		private var loginBtn:Button;
		private var loginName:TextInput;
		private var loginPasswd:TextInput;
		private var createAccTxt:CustomTextfield;
		
		public function CreateAccountView()
		{
			super();
		}
		
		private var init_y:int = -300;
		
		protected override function createChildren():void
		{
			super.createChildren();
			
			graphics.beginFill(0xff00ff, 0.7);
			graphics.drawRoundRect(0,0, 400, 250, 20);
			graphics.endFill();
			
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
			loginBtn.label = "创建角色";
			loginBtn.move(30,loginPasswd.y+40);
			addChild(loginBtn);
			
			createAccTxt = new CustomTextfield();
			createAccTxt.htmlText = "<font color='"+ColorUtil.GREEN_S+"'>返回<u><a href='event:1'>登录界面</a></u></font>";
			createAccTxt.move(0,loginBtn.y+40);
			createAccTxt.mouseEnabled = true;
			addChild(createAccTxt);
			
			y = init_y;
		}
		
		private function onHideCreateAccountHandler(e:MouseEvent):void
		{
			TweenLite.to(this, 1, {y:init_y});
		}
		
		protected function onCreateAccHandler(event:MouseEvent):void
		{
			eventManager.dispatchToController(new ParamEvent(LoginEvent.CREATE_ACCOUNT,
				{loginName:loginName.text, loginPassword:loginPasswd.text}));
		}
		
		public override function onShowHandler():void
		{
			super.onShowHandler();
			
			loginBtn.addEventListener(MouseEvent.CLICK, onCreateAccHandler);
			createAccTxt.addEventListener(MouseEvent.CLICK, onHideCreateAccountHandler);
		}
		
		public override function onHideHandler():void
		{
			super.onHideHandler();
			
			loginBtn.removeEventListener(MouseEvent.CLICK, onCreateAccHandler);
			createAccTxt.removeEventListener(MouseEvent.CLICK, onHideCreateAccountHandler);
		}
	}
}