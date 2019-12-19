#define CALCULATOR_H

#include <gtk/gtk.h>

typedef struct calbutton

{

    char *value;

    GtkWidget *button;

}calbutton;



/*function prototype*/

gint delete_event(GtkWidget *window, GdkEvent *event, gpointer data);

void create_num_button(GtkWidget *box, GtkWidget *button, char* button_num);

void click_number(GtkWidget *widget, gpointer data);

void clear(GtkWidget *widget, gpointer data);

void submit(GtkWidget *widget, gpointer data);

void add(GtkWidget *widget, gpointer data);

void sub(GtkWidget *widget, gpointer data);

void mul(GtkWidget *widget, gpointer data);

void division(GtkWidget *widget, gpointer data);

void click_symbol(GtkWidget *widget, gpointer data);

int calculator(char symbol);



#include <stdlib.h>

#include <stdio.h>

#include <string.h>

GtkWidget *entry;

int first_number = -1;

int second_number = -1;

char symbol = '+';

int need_clear = 0;

int has_press_symbol = 0;

int has_press_number = 0;

int main(int argc, char *argv[])

{

    /*init window*/

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(delete_event), NULL);

    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    gtk_window_set_title(GTK_WINDOW(window), "calculator");

    gtk_container_set_border_width(GTK_CONTAINER(window), 10);



    /*init layout*/

    GtkWidget *vbox = gtk_vbox_new(FALSE, 0);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show(vbox);



    

    GtkWidget *hbox1 = gtk_hbox_new(FALSE, 0);

    gtk_container_add(GTK_CONTAINER(vbox), hbox1);



    entry = malloc(sizeof(GtkWidget));

    memset(entry, 0, sizeof(GtkWidget));

    entry = gtk_entry_new();

    gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);

    gtk_widget_set_direction(entry, GTK_TEXT_DIR_RTL);

    gtk_entry_set_text(GTK_ENTRY(entry), "0");

    gtk_box_pack_start(GTK_BOX(hbox1), entry, TRUE, TRUE, 0);

    gtk_widget_show(entry);



    GtkWidget *button = gtk_button_new_with_label("초기화");

    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(clear), NULL);

    gtk_box_pack_start(GTK_BOX(hbox1), button, TRUE, TRUE, 0);

    gtk_widget_show(button);

    gtk_widget_show(hbox1);



    

    GtkWidget *hbox2 = gtk_hbox_new(FALSE, 0);

    gtk_container_add(GTK_CONTAINER(vbox), hbox2);

    create_num_button(hbox2, button, "7");

    create_num_button(hbox2, button, "8");

    create_num_button(hbox2, button, "9");



    

    button = gtk_button_new_with_label("+");

    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(click_symbol), (gpointer)'+');

    gtk_box_pack_start(GTK_BOX(hbox2), button, TRUE, TRUE, 0);

    gtk_widget_show(button);

    gtk_widget_show(hbox2);





    

    GtkWidget *hbox3 = gtk_hbox_new(FALSE, 0);

    gtk_container_add(GTK_CONTAINER(vbox), hbox3);

    create_num_button(hbox3, button, "4");

    create_num_button(hbox3, button, "5");

    create_num_button(hbox3, button, "6");



    

    button = gtk_button_new_with_label("-");

    gtk_box_pack_start(GTK_BOX(hbox3), button, TRUE, TRUE, 0);

    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(click_symbol), (gpointer)'-');

    gtk_widget_show(button);

    gtk_widget_show(hbox3);



    

    GtkWidget *hbox4 = gtk_hbox_new(FALSE, 0);

    gtk_container_add(GTK_CONTAINER(vbox), hbox4);

    create_num_button(hbox4, button, "1");

    create_num_button(hbox4, button, "2");

    create_num_button(hbox4, button, "3");



    

    button = gtk_button_new_with_label("*");

    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(click_symbol), (gpointer)'*');

    gtk_box_pack_start(GTK_BOX(hbox4), button, TRUE, TRUE, 0);

    gtk_widget_show(button);

    gtk_widget_show(hbox4);



    

    GtkWidget *hbox5 = gtk_hbox_new(FALSE, 0);

    gtk_container_add(GTK_CONTAINER(vbox), hbox5);

    create_num_button(hbox5, button, "0");

    button = gtk_button_new_with_label("=");

    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(submit), NULL);

    gtk_box_pack_start(GTK_BOX(hbox5), button, TRUE, TRUE, 0);

    gtk_widget_show(button);



    

    button = gtk_button_new_with_label("/");

    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(click_symbol), (gpointer)'/');

    gtk_box_pack_start(GTK_BOX(hbox5), button, TRUE, TRUE, 0);

    gtk_widget_show(button);

    gtk_widget_show(hbox5);



    gtk_widget_show(window);

    gtk_main();

    exit(0);

}







gint delete_event(GtkWidget *window, GdkEvent *event, gpointer data)

{

    gtk_main_quit();

    return FALSE;

}





void create_num_button(GtkWidget *box, GtkWidget *button, char* button_num)

{

    button = gtk_button_new_with_label(button_num);

    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(click_number), (gpointer)button_num);

    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    gtk_widget_show(button);

}



void click_number(GtkWidget *widget, gpointer data)

{

    if(need_clear)

    {

        gtk_entry_set_text(GTK_ENTRY(entry), "0");

        need_clear = 0;

    }

    char* button_num = (char*)data;

    int entry_int = get_entry_value();

    if(entry_int == 0)

        gtk_entry_set_text(GTK_ENTRY(entry), button_num);

    else

        gtk_entry_append_text(GTK_ENTRY(entry), button_num);

    has_press_number = 1;

    has_press_symbol = 0;

}







void clear(GtkWidget *widget, gpointer data)

{

    gtk_entry_set_text(GTK_ENTRY(entry), "0");

    first_number = -1;

    second_number = -1;

    has_press_number = 0;

    has_press_symbol = 0;

    need_clear = 0;

    symbol = '+';

}



void click_symbol(GtkWidget *widget, gpointer data)

{

    if(has_press_symbol == 0)

    {

        if(second_number == -1 && first_number == -1 && has_press_number == 0)

        {}

        else if(second_number == -1 && first_number == -1 && has_press_number == 1)

        {

            first_number = get_entry_value();

            has_press_number = 0;

        }

        else if(second_number == -1 && first_number != -1 && has_press_number == 1)

        {

            second_number = get_entry_value();

            first_number = calculator(symbol);

            second_number = -1;

            char* result_string = malloc(sizeof(char) * 10);

            memset(result_string, 0, 10);

            sprintf(result_string, "%d", first_number);

            gtk_entry_set_text(GTK_ENTRY(entry), result_string);

        }

        symbol = (char)data;

        need_clear = 1;

        has_press_symbol = 1;

    }

}



void submit(GtkWidget *widget, gpointer data)

{

    second_number = get_entry_value();

    int result = calculator(symbol);

    second_number = -1;

    has_press_symbol = 0;

    has_press_number = 0;

    first_number = result;

    char* result_string = malloc(sizeof(char) * 10);

    memset(result_string, 0, 10);

    sprintf(result_string, "%d", result);

    gtk_entry_set_text(GTK_ENTRY(entry), result_string);

}



int calculator(char symbol)

{

    int result;

    switch(symbol)

    {

        case '+':

            result = first_number + second_number;

            break;

        case '-':

            result = first_number - second_number;

            break;

        case '*':

            result = first_number * second_number;

            break;

        case '/':

            result = first_number / second_number;

            break;

        default:

            printf("error\n");

            return;

    }

    return result;

}





int get_entry_value()

{

    const char* entry_string = gtk_entry_get_text(GTK_ENTRY(entry));

    int entry_int = *entry_string - '0';

    entry_string++;

    while(*entry_string != '\0')

    {

        entry_int = entry_int * 10 + (*entry_string - '0');

        entry_string++;

    }

    return entry_int;

}
