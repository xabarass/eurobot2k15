/**
	Izvlacenje iz startne pozicije.
*/

function onRun(){
	Logger.debug('running task: start move');
	
	Logger.debug(Manager.getColor());
	
	CommandChain(new SleepCommand(2000)) // 2000
	.then(new ActuatorCommand('EnemyDetector', 'StartSensor'))
	.then(new MoveForward(350)) // TO_EDIT // 350
	.then(Commands.finish_task)
	.execute();
}

function onPause(){}

Manager.updateState("Ready");
