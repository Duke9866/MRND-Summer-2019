from django.contrib import messages
from django.contrib.auth import authenticate, login, logout
from django.views import View
from django.shortcuts import render, redirect
from todoapp.forms import *
from django.contrib.auth.models import User

from rest_framework.authtoken.views import ObtainAuthToken
from rest_framework.authtoken.serializers import AuthTokenSerializer
from rest_framework.authtoken.models import Token
from rest_framework.response import Response

class CustomAuthToken(ObtainAuthToken):

    def post(self, request, *args, **kwargs):
        serializer = AuthTokenSerializer(data=request.data)
        serializer.is_valid(raise_exception=True)
        user = serializer.validated_data['user']
        token, created = Token.objects.get_or_create(user=user)
        return Response({
            'token': token.key,
            'username': user.username,
        })

def logout_user(request):
    logout(request)
    return redirect('login')

class LoginController(View):

    def get(self, request, *args, **kwargs):
        if request.user.is_authenticated:
            return redirect('colleges')

        login_form = LoginForm()

        return render(
            request,
            template_name='login.html',
            context={
                'login_form': login_form,
                'title': 'Login page | Dinakaran\'s app'
            }
        )

    def post(self, request, *args, **kwargs):
        login_form = LoginForm(request.POST)
        if login_form.is_valid():
            username = login_form.cleaned_data['username']
            password = login_form.cleaned_data['password']
            user = authenticate(
                request,
                username=username,
                password=password
            )
            if user is not None:
                login(request, user)
                return redirect('colleges')
            else:
                messages.error(request, 'invalid credentials')
        return render(
            request,
            template_name='login.html',
            context={
                'login_form': login_form,
                'title': 'Login page | Dinakaran\'s app',
            }
        )

class SignUpController(View):

    def get(self, request, *args, **kwargs):
        signup_form = SignUpForm()

        return render(
            request,
            template_name='signup.html',
            context={
                'signup_form': signup_form,
                'title': 'SignUp page | Dinakaran\'s app'
            }
        )

    def post(self, request, *args, **kwargs):
        signup_form = SignUpForm(request.POST)
        if signup_form.is_valid():
            user = User.objects.create_user(**signup_form.cleaned_data)
            user.save()

        return redirect('login')
