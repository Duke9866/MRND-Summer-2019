from onlineapp.models import *
from rest_framework import serializers

class ApiMockTest1Serializer(serializers.ModelSerializer):
    class Meta:
        model = MockTest1
        fields = ('id', 'problem1', 'problem2', 'problem3', 'problem4')

class ApiStudentSerializer(serializers.ModelSerializer):
    class Meta:
        model = Student
        fields = ('id', 'name', 'dob', 'email', 'db_folder', 'dropped_out', 'college')


class ApiCollegeSerializer(serializers.ModelSerializer):
    class Meta:
        model = College
        fields = ('id', 'name', 'location', 'acronym', 'contact')

class ApiStudentDetailsSerializer(serializers.ModelSerializer):
    mocktest1 = ApiMockTest1Serializer(read_only=False, many=False)

    class Meta:
        model = Student
        fields = ('id', 'name', 'dob', 'email', 'db_folder', 'dropped_out', 'college', 'mocktest1')

    def create(self, validated_data):
        mock_vals = validated_data.pop('mocktest1')
        mock = MockTest1(**mock_vals)

        student = Student(**validated_data)
        student.college_id =self.context['college_id']
        student.save()

        mock.total = 0
        for i in range(1, 5):
            mock.total += int(mock_vals['problem' + str(i)])
        mock.student = student
        mock.save()

        return student

    def update(self, instance, validated_data):
        mock_obj = MockTest1.objects.get(student_id=instance.id)
        mock_obj.problem1 = validated_data['mocktest1']['problem1']
        mock_obj.problem2 = validated_data['mocktest1']['problem2']
        mock_obj.problem3 = validated_data['mocktest1']['problem3']
        mock_obj.problem4 = validated_data['mocktest1']['problem4']
        mock_obj.total = sum([
            mock_obj.problem1,
            mock_obj.problem2,
            mock_obj.problem3,
            mock_obj.problem4
        ])
        mock_obj.save()

        instance.name = validated_data.get('name', instance.name)
        instance.email = validated_data.get('email', instance.email)
        instance.db_folder = validated_data.get('db_folder',instance.db_folder)
        instance.dropped_out = validated_data.get('dropped_out',instance.dropped_out)
        instance.college = College.objects.get(id=self.context['college_id'])

        return instance
