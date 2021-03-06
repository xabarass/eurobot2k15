/**
	Istovara casu na nasu startnu poziciju.
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

var distance = 350;

function onRun()
{
	CommandChain(Commands.pf_move(Config.prilazna))
	.then(new RotateTo(Config.orientation))
	.then(new MoveForward(distance))
	.catch(Commands.ready_after(7000))
	.then(new ActuatorCommand('LiftCenter','Unload'))
	.then(Commands.set_world_state('nosi_casu', 'false'))
	.then(new SetSpeedMotion(70))
	.then(new MoveForward(-distance))
	.then(new SetSpeedMotion(Config.default_speed))
	.then(Commands.finish_task)
	.execute();
}

function onPause(){}

Manager.updateState("Ready");