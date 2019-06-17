from django.views import View
from django.shortcuts import render, redirect, get_object_or_404
from django.urls import resolve

from onlineapp.models import *
from onlineapp.forms import AddStudent, AddMockTest1

from rest_framework import status
from rest_framework.decorators import api_view
from rest_framework.response import Response as ApiResponse
from rest_framework.views import APIView
from onlineapp.serializers import ApiStudentSerializer, ApiStudentDetailsSerializer

from rest_framework.authentication import SessionAuthentication, BasicAuthentication, TokenAuthentication
from rest_framework.permissions import IsAuthenticated

class ApiStudentView(APIView):
    authentication_classes = (SessionAuthentication, BasicAuthentication, TokenAuthentication)
    permission_classes = (IsAuthenticated,)

    def get(self,request,*args,**kwargs):
        try:
            college = College.objects.get(id=kwargs.get('pk'))
            if not kwargs.get('sk'):
                student = Student.objects.filter(college_id=college.id).all()
                serializer = ApiStudentSerializer(student, many=True)
            else:
                # student = Student.objects.filter(college_id=college.id).get(id=kwargs.get('sk'))
                student = get_object_or_404(Student, id=kwargs.get('sk'))
                serializer = ApiStudentSerializer(student)
            return ApiResponse(serializer.data)
        except Exception:
            return ApiResponse(status=status.HTTP_400_BAD_REQUEST)

    def post(self,request,*args,**kwargs):
        try:
            student = ApiStudentDetailsSerializer(data=request.data,context={'college_id':kwargs.get('pk')})
            if student.is_valid():
                student.save()
                return ApiResponse(student.data,status=status.HTTP_201_CREATED)
        except Exception:
            return ApiResponse(status=status.HTTP_400_BAD_REQUEST)

    def delete(self, request, pk, sk, format=None):
        try:
            student = get_object_or_404(Student, pk=sk)
            student.delete()
            return ApiResponse(status=status.HTTP_204_NO_CONTENT)
        except Exception:
            return ApiResponse(status=status.HTTP_400_BAD_REQUEST)


    def put(self, request,*args,**kwargs):
        try:
            student = get_object_or_404(Student,id=kwargs.get('sk'))
            serializer = ApiStudentDetailsSerializer(student, data=request.data,context={'college_id':kwargs.get('pk'),'sk':kwargs.get('sk')})
            if serializer.is_valid():
                serializer.save()
                return ApiResponse(serializer.data)
            return ApiResponse(serializer.errors, status=status.HTTP_400_BAD_REQUEST)
        except Exception:
            return ApiResponse(status=status.HTTP_400_BAD_REQUEST)


class AddStudentView(View):
    def get(self, request, *args, **kwargs):
        permissions=request.user.get_all_permissions()
        student_form = AddStudent()
        mocktest1_form = AddMockTest1()

        if resolve(request.path_info).url_name == 'delete_student':
            # student = Student.objects.get(id=kwargs.get('pk2')).delete()
            college = get_object_or_404(College, id=kwargs.get('pk1'))
            student = get_object_or_404(Student, id=kwargs.get('pk2'))
            student.delete()
            return redirect('college_id', id=college)

        if resolve(request.path_info).url_name == 'edit_student':
            # college = College.objects.get(id=kwargs.get('pk1'))
            # student = Student.objects.get(id=kwargs.get('pk2'))

            student = get_object_or_404(Student, id=kwargs.get('pk2'))
            mocktest1 = MockTest1.objects.get(student=student)

            student_form = AddStudent(instance=student)
            mocktest1_form = AddMockTest1(instance=mocktest1)

        return render(
            request,
            template_name='add_student.html',
            context={
                'student_form': student_form,
                'mocktest1_form': mocktest1_form,
                'title': 'Register student | Dinakaran\'s app',
                'permissions': permissions,
            }
        )

    def post(self, request, *args, **kwargs):
        if resolve(request.path_info).url_name == 'edit_student':
            # college = College.objects.get(id=kwargs.get('pk1'))
            # student = Student.objects.get(id=kwargs.get('pk2'))

            college = get_object_or_404(College, id=kwargs.get('pk1'))
            student = get_object_or_404(Student, id=kwargs.get('pk2'))
            mocktest1 = MockTest1.objects.get(student=student)

            student_form = AddStudent(request.POST, instance=student)
            mocktest1_form = AddStudent(request.POST, instance=mocktest1)
            if student_form.is_valid():
                student_form.save(commit=False)
                student.college = college
                student.save()

                if mocktest1_form.is_valid():
                    mocktest1_form.save(commit=False)
                    mocktest1 = MockTest1.objects.get(student=student)

                    # mocktest1.total = mocktest1.problem1 + mocktest1.problem2 + mocktest1.problem3 + mocktest1.problem4
                    mocktest1.total = sum(mocktest1_form.cleaned_data().values())

                    mocktest1.save()

                return redirect('colleges')

        student_form = AddStudent(request.POST)
        mocktest1_form = AddMockTest1(request.POST)

        if student_form.is_valid():
            college = College.objects.get(id=kwargs.get('pk1'))

            student = student_form.save(commit=False)
            student.college = college
            student.save()

            if mocktest1_form.is_valid():
                mocktest1 = mocktest1_form.save(commit=False)
                mocktest1.student = student
                mocktest1.total = sum(mocktest1_form.cleaned_data.values())
                mocktest1.save()

            return redirect('college_id', pk=college.id)

        return render(
            request,
            template_name='add_student.html',
            context={
                'student_form': student_form,
                'mocktest1_form': mocktest1_form,
                'title': 'Register student | Dinakaran\'s app',
            }
        )