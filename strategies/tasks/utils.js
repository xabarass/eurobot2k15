/**
	Globalne funkcije ukljucene u svaki task.
*/

/**
	Motion utils.
*/

var Geometry = 
{
	'euclid_dist':function(point1, point2)
	{
		return Math.sqrt(Math.pow(point1.x - point2.x, 2) + Math.pow(point1.y - point2.y, 2));
	},
	'nearest_point':function(current_position, points)
	{
		return Math.min.apply(Math, points.map(function(point)
			{
				return Geometry.euclid_dist(current_position, point);
			}));
	},
	'nearest_point_name':function(current_position, points_map) // NEZNAM DAL RADI
	{
		var first_point = points_map.values().next().value;
		var first_key = points_map.keys().next().value;
		
		var min_val = Geometry.euclid_dist(current_position, first_point);
		var nearest_name = first_key;
		
		for(var current_key in points_map)
		{
			if(Geometry.euclid_dist(current_position, points_map[current_key]) < min_val)
			{
				nearest_name = current_key;
			}
		}
		return nearest_name;
	}
}

function Point2D(x,y)
{
	this.x = x;
	this.y = y;
}

var Motion = 
{
	'invert_x':function(points)
	{
		var ret = {};
		for(var point_name in points)
		{
			ret[point_name] = {'x':-(points[point_name].x), 'y':points[point_name].y};
		}
		return ret;
	},
	'FORWARD':1,
	'BACKWARD':-1,
	'AUTO':1, // za sad napred
}

/**
Config utils.
*/

var Config = 
{
	'color':'COLOR_NOT_SET',
	'setup':null,
	// globalna podesavanja
	'default_speed':'160',
}

function onSetup(color) // standardna onSetup, wrapuje Config.setup() i setuje Config.color
{
	Config.color = color;
	if(Config.setup) Config.setup();
}

/**
Command chaining utils.
*/

function CommandChainNode(new_command)
{
	this._first = this; // prvi cvor u lancu komandi
	this._next = null; // sledeci cvor u lancu
	this._previous = null; // prethodni cvor u lancu
	this._command = new_command; // komanda koja treba da se posalje
	this._failure_callback = null; // poziva se ako komanda ne uspe, ako je definisan
	this._progress_callback = null; // poziva se kao progres za komandu, ako je definisan
	this.then = function(next_object) // ubacuje novu komandu ili lanac komandi u ovaj lanac i vraca novi cvor (next_object moze biti komanda ili CommandChainNode)
	{
		if(next_object instanceof CommandChainNode) // dobili smo novi lanac
		{
			return this._chain(next_object);
		}
		else // dobili smo komandu (funkciju ili objekat)
		{
			return this._chain(new CommandChainNode(next_object));
		}
	};
	this._chain = function(new_node) // ulancava novi cvor u lanac i vraca novi cvor
	{
		var new_node_first = new_node._first;
		
		new_node._first = this._first; // zameni _first ovom
		var current = new_node;
		while(current._previous) // i svim prethodnim
		{
			current = current._previous;
			current._first = this._first;
		}
		
		if(this._next) // ako vec postoji _next umetni new_node u lanac
		{
			this._next._previous = new_node;
			new_node._next = this._next;
			if(!new_node._previous) new_node._failure_callback = this._failure_callback; // nasledjuje failure callback (sem ako je u pitanju lanac)
		}
		
		this._next = new_node_first;
		new_node_first._previous = this;
		return new_node;
	};
	this.failure = function(failure_callback) // podesava failure_callback za trenutni cvor i vraca trenutni cvor
	{
		this._failure_callback = failure_callback;
		return this;
	};
	this.progress = function(progress_callback) // podesava progress_callback za trenutni cvor i vraca trenutni cvor
	{
		this._progress_callback = progress_callback;
		return this;
	};
	this.catch = function(catch_callback) // podesava failure_callback za trenutni cvor i sve prethodne ukoliko nemaju vec podesen i vraca trenutni cvor
	{
		if(!this._failure_callback)
		{
			this._failure_callback = catch_callback;
		}
		if(this._previous)
		{
			this._previous.catch(catch_callback);
		}
		return this;
	};
	this.execute = function() // pocinje izvrsavanje lanca komandi od pocetka
	{
		this._first._execute_self();
	};
	this._execute_self = function(prev_result) // izvrsava trenutni cvor
	{
		// success_fun
		var success_fun = function(_prev_result) // success
		{
			if(this._next) this._next._execute_self(_prev_result); // ako postoji sledeci cvor izvrsava se
		}.bind(this);
		
		// failure_fun
		if(this._failure_callback && this._failure_callback != 'ignore') // ako je definisan callback pozovi callback
		{
			var failure_fun = function()
			{
				var fail_result = this._failure_callback.bind(this)();
				if(fail_result == 'continue') // ako callback oce da ignorise gresku pozovi success
				{
					success_fun();
				}
			}.bind(this);
		}
		else // ako nije definisano ignorisemo failure i pozivamo success
		{
			var failure_fun = success_fun;
		}
		
		// progress_fun
		if(this._progress_callback)
		{
			var progress_fun = this._progress_callback;
		}
		else
		{
			var progress_fun = function(){};
		}
		
		// execute
		if(typeof(this._command) === "function") // function command
		{
			try
			{
				var result = this._command.bind(this)(prev_result);
				success_fun(result);
			}
			catch(err)
			{
				if(err === 'command_error') failure_fun(); // ako baci 'command_error' tretiraj kao fail
				else
				{
					Logger.error('Task has thrown an exception.');
					Manager.updateState("Impossible"); // ako baci neki drugi exception suspenduj
				}
			}
		}
		else // async command
		{
			Command.send(this._command, success_fun, failure_fun, progress_fun);
		}
	};
	this.ignore_failure = function() // ignorisi fail na ovom cvoru, pozovi success u svakom slucaju
	{
		this._failure_callback = 'ignore';
		return this;
	};
	this.abort = function() // prekini dalje izvrsavanje ovog lanca
	{
		this._next = null;
	};
	this.alter = function(next_object) // izmeni lanac, ostatak lanca se odbacuje i nadovezuje se nova komanda/lanac komandi
	{
		this.abort();
		return this.then(next_object);
	};
}

function CommandChain(first_command)
{
	return new CommandChainNode(first_command);
}

var Commands = // convinience helper objekat sa funkcijama koje mogu da se proslede u .then()
{
	'do_nothing':function(){},
	'finish_task':function(){Manager.updateState("Finished");}, // Commands.finish_task
	'suspend_task':function(){Manager.updateState("Suspended");}, // Commands.suspend_task
	'wake_up_after':function(timeout, wake_up) // Commands.wake_up_after(timeout, wake_up)
	{
		return function(){Task.wake_up_after(timeout, wake_up);};
	},
	'ready_after':function(timeout) // Commands.ready_after(timeout)
	{
		return function(){Task.ready_after(timeout);};
	},
	'set_world_state':function(key, value) // Commands.ready_after(timeout)
	{
		return function(){Manager.setWorldState(key, value);};
	},
	'get_world_state':function(key) // Commands.ready_after(timeout)
	{
		return function(){return Manager.getWorldState(key);};
	},
	'pf_move':function(point, direction)
	{
		if(direction) var _dir = direction;
		else var _dir = Motion.AUTO;
		return new MoveToPosition(point.x, point.y, _dir, true);
	},
	'log_position':function()
	{
		this
		.then(new GetMotionState())
		.then(function(msg)
		{
			Logger.debug('logging position: x: '+msg.x+' y: '+msg.y);
		});
	},
}

//////////////////////////
	
/**
	Task utils.
*/

var Task = 
{
	'sleeping':false,
	'running':false,
	'suspend':function()
	{
		if(this.running) Manager.updateState("Suspended");
	},
	'ready':function()
	{
		if(!this.running) Manager.updateState("Ready");
	},
	'wake_up_after':function(timeout, wake_up)
	{
		Task.sleeping = true;
		CommandChain(new SleepCommand(timeout))
		.then(function()
		{
			Task.sleeping = false;
			wake_up();
		})
		.execute();
		Manager.updateState("Suspended");
	},
	'ready_after':function(timeout)
	{
		Task.wake_up_after(timeout, function()
		{
			Manager.updateState("Ready");
		});
	}
}

////////////////////////////////////

/**
	Lift utils.
*/

var Lift = 
{
	'item_count':
	{
		'LiftLeft':0,
		'LiftRight':0,
		'LiftCenter':0,	
	},
	'unloading_items':
	{
		'LiftLeft':false,
		'LiftRight':false,
		'LiftCenter':false,
	},
	'max_items':
	{
		'LiftLeft':4,
		'LiftRight':4,
		'LiftCenter':1,
	},
	'_subscribed':false,
	'subscribe':function()
	{
		if(!this._subscribed) // ako nisi vec subscribuj se
		{
			Notification.subscribe("LiftNotification", function(msg)
			{
				//Logger.debug('Lift notif side:'+msg.side+' count:'+msg.count);
				switch(msg.side) 
				{
					case 1: Lift.item_count['LiftRight'] = msg.count; break;
					case 2: Lift.item_count['LiftLeft'] = msg.count; break;
					case 3: Lift.item_count['LiftCenter'] = msg.count; break; // 3??
				}
				
				if(Lift._update_callback) Lift._update_callback(); // ako je definisan callback pozovi ga
			});
			this._subscribed = true;
		}
	},
	'_update_callback':null,
	'on_update':function(update_callback)
	{
		this._update_callback = update_callback;
		this.subscribe();
	},
	'unloading':function(lift)
	{
		this.unloading_items[lift] = true;
		this.subscribe();
	},
	
	'has_room_for':function(number, lift)
	{
		if(lift == 'LeftOrRight')
		{
			return (this.max_items['LiftLeft'] - this.item_count['LiftLeft']) + // kolko ima mesta u levom
				   (this.max_items['LiftRight'] - this.item_count['LiftRight']) // kolko ima mesta u desnom
				   								>= number; // ima mesta koliko nam treba ili vise
		}
		else
		{
			return this.max_items[lift] - this.item_count[lift] >= number;
		}
	},
	
	'has_items_to_unload':function()
	{
		return this.has_items_to_unload_with('LiftLeft') || this.has_items_to_unload_with('LiftRight') || this.has_items_to_unload_with('LiftCenter');
	},
	'has_items_to_unload_with':function(lift)
	{
		//Logger.debug('unloading '+this.unloading_items[lift]);
		return this.unloading_items[lift] && this.has_items_in(lift);
	},
	'has_items_in':function(lift)
	{
		//Logger.debug('has_items_in '+lift+' '+this.item_count[lift]);
		return this.item_count[lift] > 0;
	},
}


/**
	Dumb path finding.
*/


	var upper_bound = 2000-790; // TODO
	var lower_bound = 790; // TODO
	
	var upper_point = {'x':1500-750,'y':2000-(790/2)}; // TODO
	var middle_point = {'x':1500-750,'y':1000}; // TODO
	var lower_point = {'x':1500-750,'y':790/2}; // TODO
	
function find_path(destination, success, error)
{	
	CommandChain(new GetMotionState())
	.success(function(msg)
	{
		if(((msg.y > upper_bound) && (destination == 'UP')) || // already up
		   ((msg.y > lower_bound && msg.y < upper_bound) && (destination == 'MIDDLE')) || // already middle
			((msg.y < lower_bound) && (destination == 'DOWN'))) // already down
		{
			success();
			return;
		}
		
		if(msg.y > upper_bound)
		{
			var first_point = upper_point;
		}
		else if(msg.y > lower_bound && msg.y < upper_bound)
		{
			var first_point = middle_point;
		}
		else // msg.y < lower_bound
		{
			var first_point = lower_point;
		}
		
		if(destination == 'UP')
		{
			var second_point = upper_point;
		}
		else if(destination == 'MIDDLE')
		{
			var second_point = middle_point;
		}
		else // DOWN
		{
			var second_point = lower_point;
		}
		
		var fx = first_point.x;
		var fy = first_point.y;
		var sx = second_point.x;
		var sy = second_point.y;
		
		if(color == 'YELLOW')
		{
			fx = -fx;
			sx = -sx;
		}
		
		CommandChain(new MoveToPosition(fx, first_point.y))
		.then(new MoveToPosition(sx, second_point.y))
		.success(success)
		.catch(error)
		.execute();
	})
	.ignore_failure()
	.execute();
}

///////////////////////////////////////////////////////////////////
