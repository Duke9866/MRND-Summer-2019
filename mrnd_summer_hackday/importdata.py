import os
import django

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'mrnd_summer_hackday.settings')
django.setup()

import csv
from iplapp.models import *

def import_matches(filename):
    seasons_data = set()
    matches_data = dict()

    with open(filename) as match_file:

        csv_reader = csv.DictReader(match_file)

        # reading the data
        for row in csv_reader:
            row['season'] = int(row['season'])
            seasons_data.add(row['season'])
            try:
                matches_data[row['season']].append(list(row.values()))
            except KeyError:
                matches_data[row['season']] = list()
                matches_data[row['season']].append(list(row.values()))

        # saving the data
        for season in seasons_data:
            Season(season=season).save()

        for season_year in matches_data:
            season = Season.objects.get(season=season_year)

            for id, _, city, date, team1, team2, toss_winner, toss_decision, result, dl_applied, \
                winner, win_by_runs, win_by_wickets, player_of_match, venue,\
                umpire1, umpire2, umpire3 in matches_data[season_year]:

                match = Match()

                match.id = id
                match.city = city
                match.date = date
                match.team1 = team1
                match.team2 = team2
                match.toss_winner = toss_winner
                match.toss_decision = toss_decision
                match.result = result
                match.dl_applied = dl_applied
                match.winner = winner
                match.win_by_runs = win_by_runs
                match.win_by_wickets = win_by_wickets
                match.player_of_match = player_of_match
                match.venue = venue
                match.umpire1 = umpire1
                match.umpire2 = umpire2
                match.umpire3 = umpire3

                match.season = season

                match.save()
                print('Matches success')


def import_deliveries(filename):


    with open(filename) as delivery_file:

        csv_reader = csv.DictReader(delivery_file)

        # reading data
        for row in csv_reader:
            delivery = Delivery()

            delivery.inning = int(row['inning'])
            delivery.batting_team = row['batting_team']
            delivery.bowling_team = row['bowling_team']
            delivery.over = row['over']
            delivery.ball = row['ball']
            delivery.batsman = row['batsman']
            delivery.non_striker = row['non_striker']
            delivery.bowler = row['bowler']
            delivery.is_super_over = row['is_super_over']
            delivery.wide_runs = row['wide_runs']
            delivery.bye_runs = row['bye_runs']
            delivery.legbye_runs = row['legbye_runs']
            delivery.noball_runs = row['noball_runs']
            delivery.penalty_runs = row['penalty_runs']
            delivery.batsman_runs = row['batsman_runs']
            delivery.extra_runs = row['extra_runs']
            delivery.total_runs = row['total_runs']
            delivery.player_dismissed = row['player_dismissed']
            delivery.dismissal_kind = row['dismissal_kind']
            delivery.fielder = row['fielder']

            delivery.match = Match.objects.get(id=row['match_id'])

            delivery.save()
            print('Deliveries success')

if __name__ == '__main__':
    import_matches('matches.csv')
    import_deliveries('deliveries.csv')
    pass
