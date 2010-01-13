#include "main.h"
#include "rhd.h"

int
main(int argc, char **argv)
{
    if(argc < 2)
	{
	    printf("Usage: smr <speed>\n");
	    exit(EXIT_FAILURE);
	}

    input in;
    output out;

    init(&in, &out);
    mission(atoi(argv[1]), &in, &out);
    term(&out);
    exit(0);
}

void
init(input * in, output * out)
{
    if(rhdConnect('w', "localhost", ROBOTPORT) != 'w')
	{
	    printf("Can't connect to rhd \n");
	    exit(EXIT_FAILURE);
	}
    printf("connected to robot \n");

    symTableElement *inputtable, *outputtable;

    if((inputtable = getSymbolTable('r')) == NULL)
	{
	    printf("Can't connect to rhd \n");
	    exit(EXIT_FAILURE);
	}
    if((outputtable = getSymbolTable('w')) == NULL)
	{
	    printf("Can't connect to rhd \n");
	    exit(EXIT_FAILURE);
	}

    in->encoder_left = getinputref("encl", input_table);
    in->encoder_right = getinputref("encl", input_table);
    in->line_sensor = getinputref("linesensor", input_table);
    in->ir_sensor = getinputref("irsensor", input_table);

    out->speed_left = getoutputref("speedl", output_table);
    out->speed_right = getoutputref("speedr", output_table);
    out->reset_motor_left = getoutputref("resetmotorr", output_table);
    out->reset_motor_right = getoutputref("resetmotorl", output_table);

    rhdSync();
}

void
term(output * out)
{
    task(task.stop, trigger.none);
    rhdSync();
    rhdDisconnect();
}

symTableElement *
getinputref(const char *sym_name, symTableElement * tab)
{
    int i;
    for(i = 0; i < getSymbolTableSize('r'); i++)
	if(strcmp(tab[i].name, sym_name) == 0)
	    return &tab[i];
    return 0;
}

symTableElement *
getoutputref(const char *sym_name, symTableElement * tab)
{
    int i;
    for(i = 0; i < getSymbolTableSize('w'); i++)
	if(strcmp(tab[i].name, sym_name) == 0)
	    return &tab[i];
    return 0;
}
