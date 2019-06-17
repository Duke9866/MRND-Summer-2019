from django import forms
from onlineapp.models import *

class AddCollege(forms.ModelForm):
    class Meta:
        model = College
        exclude = ['id']
        widgets = {
            'name': forms.TextInput(attrs={'class': 'input', 'placeholder': 'Enter college name'}),
            'location': forms.TextInput(attrs={'class': 'input', 'placeholder': 'Enter college location'}),
            'acronym': forms.TextInput(attrs={'class': 'input', 'placeholder': 'Enter college acronym'}),
            'contact': forms.EmailInput(attrs={'class': 'input', 'placeholder': 'Enter college contact'})
        }

