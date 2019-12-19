#include <gtk/gtk.h>
#include <stdio.h>
GtkWidget *checkbutton;
GtkWidget *togglebutton;
GtkWidget *radiobutton1, *radiobutton2;
void quit(GtkWidget *window, gpointer data)
{
    gtk_main_quit();
}

void add_widget_with_label(GtkContainer *box, gchar *caption, GtkWidget *widget)
{
    GtkWidget *label = gtk_label_new(caption);
    GtkWidget *hbox = gtk_hbox_new(TRUE, 4);
    gtk_container_add(GTK_CONTAINER(hbox), label);
    gtk_container_add(GTK_CONTAINER(hbox), widget);
    gtk_container_add(box, hbox);
}

void print_active(char *name, GtkToggleButton *button)
{
    gboolean active =
        gtk_toggle_button_get_active(button);
    printf("%s 는 현재 %s 상태입니다.\n", name, active ? "활성" : "비활성");
}

void button_clicked(GtkWidget *button, gpointer data)
{
    print_active("체크버튼",
                 GTK_TOGGLE_BUTTON(checkbutton));
    print_active("토글버튼",
                 GTK_TOGGLE_BUTTON(togglebutton));
    print_active("라디오버튼1",
                 GTK_TOGGLE_BUTTON(radiobutton1));
    print_active("라디오버튼2",
                 GTK_TOGGLE_BUTTON(radiobutton2));
    printf("\n");
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *vbox;
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "버튼즈");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(quit), NULL);
    button = gtk_button_new_with_label("OK");
    checkbutton = gtk_check_button_new();
    togglebutton = gtk_toggle_button_new_with_label("토글");
    radiobutton1 = gtk_radio_button_new(NULL);
    radiobutton2 = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radiobutton1));
    vbox = gtk_vbox_new(TRUE, 4);
    add_widget_with_label(GTK_CONTAINER(vbox), "체크버튼:", checkbutton);
    add_widget_with_label(GTK_CONTAINER(vbox), "토글버튼:", togglebutton);
    add_widget_with_label(GTK_CONTAINER(vbox), "라디오버튼 1:", radiobutton1);
    add_widget_with_label(GTK_CONTAINER(vbox), "라디오버튼 2:", radiobutton2);
    add_widget_with_label(GTK_CONTAINER(vbox), "버튼:", button);
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
