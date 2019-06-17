from django.db import models
from django.contrib.auth.models import User

# Create your models here.

class ToDoList(models.Model):
    name = models.CharField(max_length=128)
    created = models.DateTimeField(null=False, blank=False)

    user = models.ForeignKey(User, on_delete=models.CASCADE)

    def __str__(self):
        return f"ToDoList:{self.name}"

class ToDoItem(models.Model):
    description = models.CharField(max_length=256)
    due_date = models.DateTimeField(null=True, blank=True)
    completed = models.BooleanField()

    todolist = models.ForeignKey(ToDoList, on_delete=models.CASCADE)

    def __str__(self):
        return f"ToDoItem:{self.description}"