from django.views import View
from django.shortcuts import render, redirect, get_object_or_404
from django.urls import resolve
from django.contrib.auth.mixins import LoginRequiredMixin

from onlineapp.models import *
from onlineapp.forms import AddCollege

from rest_framework import status
from rest_framework.decorators import api_view
from rest_framework.response import Response as ApiResponse
from rest_framework.views import APIView
from onlineapp.serializers import ApiCollegeSerializer, ApiStudentSerializer

from rest_framework.authentication import SessionAuthentication, BasicAuthentication, TokenAuthentication
from rest_framework.permissions import IsAuthenticated

# @api_view(['GET'])
# def ApiCollegeView(request, *args, **kwargs):
#
#     if request.method == 'GET':
#         if kwargs:
#             college = get_object_or_404(College, id=kwargs.get('pk'))
#             students = list(college.student_set.order_by('-mocktest1__total'))
#             college_serializer = CollegeSerializer(college)
#             return ApiResponse(college_serializer.data)
#         colleges = College.objects.all()
#         college_serializer = CollegeSerializer(colleges, many=True)
#         return ApiResponse(college_serializer.data)

class ApiCollegeView(APIView):
    authentication_classes = (SessionAuthentication, BasicAuthentication, TokenAuthentication)
    permission_classes = (IsAuthenticated,)

    # display data
    def get(self, request, *args, **kwargs):
        try:
            if kwargs:
                college = College.objects.get(id=kwargs.get('pk'))
                college_serializer = ApiCollegeSerializer(college)
                return ApiResponse(college_serializer.data, status=status.HTTP_200_OK)
            else:
                colleges = College.objects.all()
                college_serializer = ApiCollegeSerializer(colleges, many=True)
                return ApiResponse(college_serializer.data, status=status.HTTP_200_OK)
        except Exception:
            return ApiResponse(status=status.HTTP_400_BAD_REQUEST)

    # add data
    def post(self, request, *args, **kwargs):
        try:
            college_serializer = ApiCollegeSerializer(data=request.data)
            if college_serializer.is_valid():
                college_serializer.save()
                return ApiResponse(college_serializer.data, status=status.HTTP_200_OK)
            else:
                return ApiResponse(status=status.HTTP_400_BAD_REQUEST)
        except Exception:
            return ApiResponse(status=status.HTTP_400_BAD_REQUEST)

    # update data
    def put(self, request, *args, **kwargs):
        try:
            college = College.objects.get(id=kwargs.get('pk'))
            college_serializer = ApiCollegeSerializer(data=request.data, instance=college)
            if college_serializer.is_valid():
                college_serializer.save()
                return ApiResponse(college_serializer.data, status=status.HTTP_200_OK)
            else:
                return ApiResponse(college_serializer.errors, status=status.HTTP_400_BAD_REQUEST)
        except Exception:
            return ApiResponse(status=status.HTTP_400_BAD_REQUEST)

    # delete data
    def delete(self, request, *args, **kwargs):
        try:
            college = College.objects.get(id=kwargs.get('pk'))
            college.delete()
            return ApiResponse(status=status.HTTP_200_OK)
        except Exception:
            return ApiResponse(status=status.HTTP_400_BAD_REQUEST)


class CollegeView(LoginRequiredMixin, View):
    login_url = '/login/'

    def get(self, request, *args, **kwargs):
        permissions=request.user.get_all_permissions()
        if kwargs:
            college = get_object_or_404(College, id=kwargs.get('pk'))
            students = list(college.student_set.order_by('-mocktest1__total'))
            return render(
                request,
                template_name='college_students.html',
                context={
                    'college': college,
                    'students': students,
                    'permissions': permissions,
                }
            )
        colleges = College.objects.all()
        return render(
            request,
            template_name='college_names.html',
            context={
                'colleges': colleges,
                'title': 'All colleges | Dinakaran\'s app',
                'permissions': permissions,
            }
        )


class AddCollegeView(LoginRequiredMixin, View):
    login_url = '/login/'
    def get(self, request, *args, **kwargs):
        form = AddCollege()

        if resolve(request.path_info).url_name == 'edit_college':
            college = get_object_or_404(College, id=kwargs.get('pk'))
            form = AddCollege(instance=college)

        if resolve(request.path_info).url_name == 'delete_college':
            college = get_object_or_404(College, id=kwargs.get('pk'))
            college.delete()
            return redirect('colleges')

        return render(
            request,
            template_name='add_college.html',
            context={
                'form': form,
                'title': 'Register college | Dinakaran\'s app',
            }
        )

    def post(self, request, *args, **kwargs):
        if resolve(request.path_info).url_name == 'edit_college':
            college = get_object_or_404(College, id=kwargs.get('pk'))
            form = AddCollege(request.POST, instance=college)
            if form.is_valid():
                form.save()
                return redirect('colleges')

        form = AddCollege(request.POST)
        if form.is_valid():
            form.save()
            return redirect('colleges')

        return render(
            request,
            template_name='add_college.html',
            context={
                'form': form,
                'title': 'Register College | Dinakaran\'s App',
            }
        )

