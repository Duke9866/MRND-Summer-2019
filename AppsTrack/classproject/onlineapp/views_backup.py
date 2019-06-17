import logging
from django.shortcuts import render
from django.http import HttpRequest, HttpResponse
from onlineapp.models import *
# Create your views here.

def hello_world(request):
    file = open('D:\Apps Course Projects\django-project\classproject\onlineapp\hello_world.html')
    response = file.read()
    return HttpResponse(response)

def clg_name(request):
    name = request.headers['clg']
    return HttpResponse(name)

def clg_names_and_acronyms(request):
    colleges = College.objects.all()
    return render(request, 'college_names.html', {'clgs': colleges})

def clg_students(request, id):
    college = College.objects.get(id=id)
    students = Student.objects.values('name', 'mocktest1__total').filter(college=college)
    return render(request, 'college_students.html', {'college': college, 'students': students})

def test_view(request):
    logger = logging.getLogger(__name__)
    logger.error('This is an error message from test view')
    logger.info('This is an info message from test view')
    return HttpResponse("This is my testing response")