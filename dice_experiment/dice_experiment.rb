load 'dice.rb'
load 'dice_games.rb'

#implemented numeric function to calculate percentage. Just for convenience
class Numeric
  def percent_of(n)
    (self.to_f / n.to_f) * 100.0
  end
end

def test_dices(n_faces = 6, sample_size = 1000, desonesty = 4)
  print "Testing dices with " + sample_size.to_s + " samples and " + n_faces.to_s + "-faced dice...\n" +
        "The last face have " + desonesty.to_s + "x more chance on desonesty dice\n\n"
  honest_dice = Dice.new(n_faces)
  dishonest_dice = Dice.new(n_faces, [1]*(n_faces-1) + [desonesty])

  count_honest = count_dishonest = 0
  sample_size.times do
    count_honest += honest_dice.roll
    count_dishonest += dishonest_dice.roll
  end

  p "Honest average result: " + (count_honest/sample_size).to_s
  p "Dishonest average result: " + (count_dishonest/sample_size).to_s
end

def experiment_gaming(n_faces = 6, sample_size = 1000, desonesty = 4)
  print "Experimenting the games with " + sample_size.to_s + " samples and " + n_faces.to_s + "-faced dice...\n" +
        "The last face have " + desonesty.to_s + "x more chance on desonesty dice\n\n"
  honest_dice = Dice.new(n_faces)
  dishonest_dice = Dice.new(n_faces, [1]*(n_faces-1) + [desonesty])

  count_honest_three_equals = count_dishonest_three_equals = 0
  count_honest_two_pairs = count_dishonest_two_pairs = 0
  sample_size.times do
    count_honest_three_equals += 1 if DiceGames.ThreeEquals(honest_dice)
    count_honest_two_pairs += 1 if DiceGames.TwoPairs(honest_dice)

    count_dishonest_three_equals += 1 if DiceGames.ThreeEquals(dishonest_dice)
    count_dishonest_two_pairs += 1 if DiceGames.TwoPairs(dishonest_dice)
  end

  p "Honest victory percentage with ThreeEquals: " + count_honest_three_equals.percent_of(sample_size).round(2).to_s + "%"
  p "Dishonest victory percentage with ThreeEquals " + count_dishonest_three_equals.percent_of(sample_size).round(2).to_s + "%"
  print "\n"
  p "Honest victory percentage with TwoPairs: " + count_honest_two_pairs.percent_of(sample_size).round(2).to_s + "%"
  p "Dishonest victory percentage with TwoPairs: " + count_dishonest_two_pairs.percent_of(sample_size).round(2).to_s + "%"
end

def estimating_some_probability(n_faces = 6, sample_size = 1000, desonesty = 4)
  print "Estimating probability of the first result being equals to third and different from second.\n" +
        "With " + sample_size.to_s + " samples and " + n_faces.to_s + "-faced dice...\n" +
        "The last face have " + desonesty.to_s + "x more chance on desonesty dice\n\n"

  honest_dice = Dice.new(n_faces)
  dishonest_dice = Dice.new(n_faces, [1]*(n_faces-1) + [desonesty])

  count_honest = count_dishonest = 0
  sample_size.times do
    count_honest += 1 if DiceGames.FirstEqualsThirdUnlikeSecond honest_dice
    count_dishonest += 1 if DiceGames.FirstEqualsThirdUnlikeSecond dishonest_dice
  end

  p "Honest probability: " + count_honest.percent_of(sample_size).round(2).to_s + "%"
  p "Dishonest probability: " + count_dishonest.percent_of(sample_size).round(2).to_s + "%"
end

print "--------------------------------------------\n\n"
test_dices 6, 100000, 5
print "\n--------------------------------------------\n\n"
experiment_gaming 6, 25000, 5
print "\n--------------------------------------------\n\n"
experiment_gaming 6, 25000, 25
print "\n--------------------------------------------\n\n"
estimating_some_probability 6, 15000, 5
print "\n--------------------------------------------\n\n"
estimating_some_probability 6, 15000, 25
print "\n--------------------------------------------\n"
