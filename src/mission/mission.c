void
mission(int speed, input * in, output * out)
{
    char finished = 0;
    do
	{
	    //Mission State Machine
	    switch (mission.state)
		{
		case state.finished:
		    task(task.stop, trigger.none, in, out);
		    finished = 1;
		    break;
		}

	    //Stop if keyboard is activated
	    ioctl(0, FIONREAD, &arg);
	    if(arg != 0)
		{
		    mission.state = state.finished;
		}
	}
    while(!finished);
}
