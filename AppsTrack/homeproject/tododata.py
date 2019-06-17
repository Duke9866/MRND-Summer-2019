import os
import django
import click
import MySQLdb

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'homeproject.settings')
django.setup()

from todoapp.models import *

todolist_names = [
    'File System',
    'Excel Sheet',
    'B+ Trees: Database',
    'Compilers',
    'Calendar',
]

todoitems_desc = [
    'Create the design',
    'Write the code',
    'Test the code',
    'Save the code in local git repo',
    'Save the code in online git repo',
]

def insert_todolist():
    global todolist_names

    for name in todolist_names:
        tdl = ToDoList(name=name)
        tdl.save()

def insert_todoitem():
    global todolist_names
    global todoitems_desc

    for name in todolist_names:
        tdl = ToDoList.objects.get(name=name)
        for description in todoitems_desc:
            tdi = ToDoItem(description=description)
            tdi.todolist = tdl
            tdi.save()

if __name__ == '__main__':
    insert_todolist()
    insert_todoitem()