# todolist form

from django import forms
from todoapp.models import *

class AddTodoList(forms.ModelForm):
    class Meta:
        model = ToDoList
        exclude = ['id', 'todolist']
        widgets = {
            'description': forms.TextInput(attrs={'class': 'input', 'placeholder': 'Enter to-do list item description'}),
            'due_date': forms.DateInput(attrs={'type': 'date'}),
            'dropped_out': forms.CheckboxInput(),
        }