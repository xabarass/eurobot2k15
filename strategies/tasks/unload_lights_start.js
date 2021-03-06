/**
	Istovara valjke na nasu startnu poziciju.
	DEPRECATED NERADI KORISTITI UNLOAD ALL
*/

var pos_y = 1000; // TO_EDIT
var pos_x = 800; // TO_EDIT

var position_colored = 
{
	'YELLOW':{'x':-pos_x, 'y':pos_y},
	'GREEN':{'x':pos_x, 'y':pos_y},
};

var orientation_colored = // TO_EDIT
{
	'YELLOW':180,
	'GREEN':0,
};

Config.setup = function()
{
	Config['prilazna'] = position_colored[Config.color];
	Config['orientation'] = orientation_colored[Config.color];
}

var distance = 250;

function onRun(){
	
	CommandChain(Commands.pf_move(Config.prilazna))
	.then(new RotateTo(Config.orientation))
	.then(new MoveForward(distance))
	.success(function()
	{
		CommandChain(new ActuatorCommand('LiftLeft','Unload')).execute();
		CommandChain(new ActuatorCommand('LiftRight','Unload')).execute();
		
		CommandChain(new SleepCommand(1000))
		.then(new MoveForward(-200))
		.then(new SetSpeedMotion(Config.default_speed))
		.success(function()
		{
			Manager.updateState("Finished");
		})
		.ignore_failure()
		.execute();
	})
	.catch(function()
	{
		Manager.updateState("Suspended");
	})
	.execute();
}

function onPause(){}

Manager.updateState("Ready");