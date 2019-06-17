from django.views import View
from django.shortcuts import render, redirect, get_object_or_404
from django.contrib.auth.mixins import LoginRequiredMixin

from todoapp.models import *
from todoapp.forms import ToDoList

class ToDoListView(LoginRequiredMixin, View):
    login_url = '/login/'

    def get(self, request, *args, **kwargs):
        permissions=request.user.get_all_permissions()
        todolist = ToDoList.objects.filter(user=request.user)