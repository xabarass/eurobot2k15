/**
	Istovara casu na polje kod protivnickog startnog polja.
*/

var pos_y = 450; // TO_EDIT
var pos_x = 450; // TO_EDIT

var position_colored = 
{
	'YELLOW':{'x':-pos_x, 'y':pos_y},
	'GREEN':{'x':pos_x, 'y':pos_y},
};

var orientation_colored = // TO_EDIT
{
	'YELLOW':45,
	'GREEN':135,
};

Config.setup = function()
{
	Config['prilazna'] = position_colored[Config.color];
	Config['orientation'] = orientation_colored[Config.color];
}

var distance = 100;

function onRun()
{
	CommandChain(Commands.pf_move(Config.prilazna))
	.then(new RotateTo(Config.orientation))
	.then(new MoveForward(distance))
	.catch(Commands.ready_after(7000))
	.then(new ActuatorCommand('LiftCenter','Unload'))
	.then(Commands.set_world_state('nosi_casu', 'false'))
	.then(new MoveForward(-distance))
	.then(Commands.finish_task)
	.execute();
}

function onPause(){}

Manager.updateState("Ready");