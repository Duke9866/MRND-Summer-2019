from django import forms

class LoginForm(forms.Form):
    username = forms.CharField(
        max_length=128,
        required=True,
        widget=forms.TextInput(attrs={'class': 'input', 'placeholder': 'Enter username'}),
    )
    password = forms.CharField(
        max_length=128,
        required=True,
        widget=forms.PasswordInput(attrs={'class': 'input', 'placeholder': 'Enter password'}),
    )

class SignUpForm(forms.Form):
    first_name = forms.CharField(
        max_length=128,
        required=True,
        widget=forms.TextInput(attrs={'class': 'input', 'placeholder': 'Enter the first name'}),
    )
    last_name = forms.CharField(
        max_length=128,
        required=True,
        widget=forms.TextInput(attrs={'class': 'input', 'placeholder': 'Enter the last name'}),
    )
    username = forms.CharField(
        max_length=128,
        required=True,
        widget=forms.TextInput(attrs={'class': 'input', 'placeholder': 'Enter the username'}),
    )
    password = forms.CharField(
        max_length=128,
        required=True,
        widget=forms.PasswordInput(attrs={'class': 'input', 'placeholder': 'Enter the password'}),
    )