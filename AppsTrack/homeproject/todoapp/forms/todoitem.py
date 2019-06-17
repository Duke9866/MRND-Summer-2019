# todoitem form

from django import forms
from todoapp.models import *

class AddTodoList(forms.ModelForm):
    class Meta:
        model = ToDoItem
        exclude = ['id']
        widgets = {
            'name': forms.TextInput(attrs={'class': 'input', 'placeholder': 'Enter to-do list name'}),
        }

