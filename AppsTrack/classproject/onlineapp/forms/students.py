from django import forms
from onlineapp.models import *

class AddStudent(forms.ModelForm):
    class Meta:
        model = Student
        exclude = ['id', 'college']
        widgets = {
            'name': forms.TextInput(attrs={'class': 'input', 'placeholder': 'Enter student name'}),
            'dob': forms.DateInput(attrs={'type': 'date'}),
            'email': forms.EmailInput(attrs={'class': 'input', 'placeholder': 'Enter student email'}),
            'db_folder': forms.TextInput(attrs={'class': 'input', 'placeholder': 'Enter student db_folder name'}),
            'dropped_out': forms.CheckboxInput(),
        }

class AddMockTest1(forms.ModelForm):
    class Meta:
        model = MockTest1
        exclude = ['id', 'total', 'student']
        widgets = {
            'problem1': forms.NumberInput(attrs={'class': 'input', 'placeholder': 'Enter problem1 score'}),
            'problem2': forms.NumberInput(attrs={'class': 'input', 'placeholder': 'Enter problem2 score'}),
            'problem3': forms.NumberInput(attrs={'class': 'input', 'placeholder': 'Enter problem3 score'}),
            'problem4': forms.NumberInput(attrs={'class': 'input', 'placeholder': 'Enter problem4 score'}),
        }