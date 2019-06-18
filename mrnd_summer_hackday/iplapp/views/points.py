from django.views import View
from django.shortcuts import render
from django.db.models import Sum, Count
from iplapp.models import *


def get_points_dict(season):
    query = Match.objects.all().values("winner").filter(season=season).annotate(count=Count('result')).order_by('-count')
    last =  Match.objects.all().filter(season=season).order_by('-id')
    count=(Match.objects.values('team1').filter(season=season).distinct().count() -1 )*2
    if(season in [2009,2008,2010]):
        leavemat=3
    else:
        leavemat=4
    points = []
    for match in query:
        match1=[]
        if (match['winner'] != None):
            match1.append(match['winner'])
            match1.append(count)
            match1.append(match['count'])
            match1.append(count - (match['count']))
            match1.append((match['count'])*2)
        if(match1 !=[]):
            points.append(match1)
    for index in range(leavemat):
        for match1 in points:
            if(match1[0]==last[index].winner):
                match1[2] -=1
                match1[3] +=1
                match1[4] -=2
    noresult=  Match.objects.all().filter(season=2019, winner=None).order_by('-id')
    for match in noresult:
        print(match.team1,match.team2)
        for match1 in points:
            if(match1[0]==match.team1 or match.team2==match1[0]):
                match1[4]+=1
    return points

class PointsView(View):

    def get(self, request, *args, **kwargs):
        seasons = Season.objects.all().order_by('-season')
        if  request.user.is_anonymous:
            current_user = None
        else:
            current_user = request.user

        if kwargs:
            season = Season.objects.get(season=kwargs.get('season'))
            points = get_points_dict(season)
            winner = Match.objects.all().filter(season=season).order_by('-id')[0].winner
        return render(
            request,
            template_name='points.html',
            context={
                'user': current_user,
                'year': season,
                'points': points,
                'winner': winner,
                'seasons': seasons,
                'title': 'IPL Clone App',
            }
        )