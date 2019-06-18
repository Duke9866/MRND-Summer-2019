from django.views import View
from django.shortcuts import render, redirect, get_object_or_404

from iplapp.models import *

class SeasonView(View):

    def get(self, request, *args, **kwargs):
        seasons = Season.objects.all().order_by('-season')
        if  request.user.is_anonymous:
            current_user = None
        else:
            current_user = request.user

        if kwargs:
            season = Season.objects.get(season=kwargs.get('season'))
            page = int(kwargs.get('page'))

            start_page = 8 * (page - 1) + 1
            end_page = 8 * page

            match = Match.objects.filter(season=season)[start_page: end_page + 1]
            count = Match.objects.filter(season=season).count()

            next = page + 1
            prev = page - 1

            if start_page == 1:
                prev = None

            if start_page == (count // 8) * 8 + 1:
                next = None

        return render(
            request,
            template_name='seasons.html',
            context={
                'user': current_user,
                'year': season,
                'seasons': seasons,
                'next': next,
                'prev': prev,
                'matches': enumerate(match, start=start_page),
                'title': 'IPL Clone App',
            }
        )