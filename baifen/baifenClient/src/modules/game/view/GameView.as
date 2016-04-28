package modules.game.view
{
	import com.greensock.TweenLite;
	import com.greensock.TweenMax;
	
	import face2wind.customUIComponents.Message;
	import face2wind.enum.LayerEnum;
	import face2wind.event.ParamEvent;
	import face2wind.event.PropertyChangeEvent;
	import face2wind.lib.ObjectPool;
	import face2wind.manager.LayerManager;
	import face2wind.manager.StageManager;
	import face2wind.view.BaseSprite;
	
	import flash.display.Sprite;
	
	import modules.game.event.GameEvent;
	import modules.game.model.GameManager;
	
	import socketCommand.customData.PokerInfo;
	import socketCommand.customData.SC10202_pokerNumList;
	
	/**
	 * 游戏主视图
	 * @author face2wind
	 */
	public class GameView extends BaseSprite
	{
		public function GameView()
		{
			super();
		}
		
		private var tablePokerLayer:BaseSprite;
		
		private var rolePokers:RolePokersView;
		private var rolePokerListChange:Boolean;
		private var pokerNumChange:Boolean;
		
		private var otherPokerViewList:Array;
		private var turnChange:Boolean;
		private var turnPoint:Sprite;
		private var _tableAllPokerViewList:Array;
		
		public function show():void
		{
			var layer:BaseSprite = LayerManager.getInstance().getLayer(LayerEnum.SCENE_LAYER);
			layer.addChild(this);
		}
		
		public function hide():void
		{
			var layer:BaseSprite = LayerManager.getInstance().getLayer(LayerEnum.SCENE_LAYER);
			if(layer.contains(this))
				layer.removeChild(this);
		}
		
		/**
		 * 此函数是视图的内容初始化函数<br/>对父类的覆盖 
		 * 
		 */		
		protected override function createChildren():void
		{
			super.createChildren();
			
			tablePokerLayer = new BaseSprite();
			addChild(tablePokerLayer);
			
			rolePokers = new RolePokersView();
			addChild(rolePokers);
			
			otherPokerViewList = [];
			_tableAllPokerViewList = [];
			
			var otherPokers:OtherPokerListView = new OtherPokerListView(false);
			addChild(otherPokers);
			otherPokerViewList.push(otherPokers);
			
			otherPokers = new OtherPokerListView();
			addChild(otherPokers);
			otherPokerViewList.push(otherPokers);
			
			otherPokers = new OtherPokerListView(false);
			addChild(otherPokers);
			otherPokerViewList.push(otherPokers);
			
			turnPoint = new Sprite();
			turnPoint.graphics.beginFill(0xff00ff);
			turnPoint.graphics.drawCircle(0,0,15);
			turnPoint.graphics.endFill();
			addChild(turnPoint);
		}
		
		/**
		 * 属性变更，更新视图 
		 * 
		 */	
		protected override function update():void
		{
			super.update();
			var manager:GameManager = GameManager.getInstance();
			if(rolePokerListChange){
				rolePokerListChange = false;
				rolePokers.pokerList = manager.curRolePokerList;
			}
			if(pokerNumChange){
				pokerNumChange = false;
				var roleIndex:int = manager.curRoleIndex;
				var playersPokerNums:Array = manager.allPlayerPokerNums.concat();
				for (var i:int = 0; i < roleIndex; i++) 
					playersPokerNums.push(playersPokerNums.shift());
				playersPokerNums.shift();// 把主角删除
				for (var j:int = 0; j < otherPokerViewList.length; j++) 
					(otherPokerViewList[j] as OtherPokerListView).pokerNum = 
						(playersPokerNums[j] as SC10202_pokerNumList).pokerNum;
			}
			if(turnChange){
				turnChange = false;
				if(manager.nextTurnPlayerIndex == manager.curRoleIndex)
					rolePokers.isMyTurn = true;
				else
					rolePokers.isMyTurn = false;
				updateTurnPointPosition();
			}
		}
		
		public function clearTablePokers():void
		{
			if(null == _tableAllPokerViewList)
				return;
			while (0 < _tableAllPokerViewList.length) 
			{
				var poker:PokerView = _tableAllPokerViewList.shift();
				if(tablePokerLayer.contains(poker))
					tablePokerLayer.removeChild(poker);
			}
		}
		
		private function updateTurnPointPosition():void
		{
			var w:Number = StageManager.getInstance().stageWidth;
			var h:Number = StageManager.getInstance().stageHeight;
			var manager:GameManager = GameManager.getInstance();
			var roleIndex:int = manager.curRoleIndex;
			var nextIndex:int = manager.nextTurnPlayerIndex;
			var positionList:Array = [[w/2,h-190],[w-190,h/2],[w/2,190],[190,h/2]];
			var realIndex:int = nextIndex-roleIndex;
			if(0 > realIndex )
				realIndex += 4;
			turnPoint.x = positionList[realIndex][0];
			turnPoint.y = positionList[realIndex][1];
		}
		
		private function onStageResize(w:Number, h:Number):void
		{
			rolePokers.move(190,h-190);
			graphics.clear();
			graphics.beginFill(0xCCE8CF);
			graphics.drawRect(0,0,w,h);
			graphics.endFill();
			
			otherPokerViewList[0].move(w-190, 0);
			otherPokerViewList[1].move(190, 0);
			otherPokerViewList[2].move(0, 0);
			
			updateTurnPointPosition();
		}
		
		protected function onManagerChangeHandle(event:PropertyChangeEvent):void
		{
			switch(event.property)
			{
				case "curRoleIndex":
				case "nextTurnPlayerIndex":
					turnChange = true;
				case "allPlayerPokerNums":
					pokerNumChange = true;
					break;
				case "curRolePokerList":
					rolePokerListChange = true;
					break;
			}
			propertyChange();
		}
		
		private function onShowCardsEffectHandle(e:ParamEvent):void
		{
			Message.show("播放出牌效果~~~");
			var w:Number = StageManager.getInstance().stageWidth;
			var h:Number = StageManager.getInstance().stageHeight;
			var positionList:Array = [[w/2,h-190],[w-190,h/2],[w/2,190],[190,h/2]];
			var index:int = e.param.index;
			var pokerList:Array = e.param.pokerList;
			var roleIndex:int = GameManager.getInstance().curRoleIndex;
			var targetIndex:int = index - roleIndex;
			if(0 > targetIndex)
				targetIndex += 4;
			var rawPosition:Array = positionList[targetIndex];
			var rawTargetPoint:Array = [Math.random()*300+90, Math.random()*200+90];
			for (var i:int = 0; i < pokerList.length; i++) 
			{
				var pinfo:PokerInfo = pokerList[i];
				var poker:PokerView = ObjectPool.getObject(PokerView);
				poker.pvo = pinfo;
				poker.move(rawPosition[0]+i*30,rawPosition[1]);
				TweenLite.to(poker, 0.7, {x:rawTargetPoint[0]+i*30,y:rawTargetPoint[1]});
				tablePokerLayer.addChild(poker);
				_tableAllPokerViewList.push(poker);
			}
		}
		
		/**
		 * 被加到显示列表时执行 
		 */		
		public override function onShowHandler():void
		{
			StageManager.getInstance().addStageResizeFunc(onStageResize,true);
			GameManager.getInstance().addEventListener(PropertyChangeEvent.PROPERTY_CHANGE,onManagerChangeHandle);
			eventManager.bindToView(GameEvent.SHOW_SEND_CARDS_EFFECT, onShowCardsEffectHandle);
		}
		
		/**
		 * 从显示列表移除时执行 
		 */		
		public override function onHideHandler():void
		{
			StageManager.getInstance().removeStageResizeFunc(onStageResize);
			GameManager.getInstance().removeEventListener(PropertyChangeEvent.PROPERTY_CHANGE,onManagerChangeHandle);
			eventManager.unbindToView(GameEvent.SHOW_SEND_CARDS_EFFECT, onShowCardsEffectHandle);
		}
	}
}