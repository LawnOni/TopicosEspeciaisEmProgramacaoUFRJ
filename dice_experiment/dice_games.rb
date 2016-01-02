module DiceGames
  def DiceGames.play(dice, n_plays)
    plays = []
    n_plays.times do
      plays += [dice.roll]
    end
    return plays
  end

  def DiceGames.ThreeEquals(dice)
    plays = play(dice, 3)
  	return (plays[0] == plays[1]) && (plays[1] == plays[2])
  end

  def DiceGames.TwoPairs(dice)
    plays = play(dice, 4)
  	return (plays[0] == plays[1]) && (plays[2] == plays[3])
  end

  def DiceGames.FirstEqualsThirdUnlikeSecond(dice)
    plays = play(dice, 3)
  	return (plays[0] == plays[2]) && (plays[0] == plays[1])
  end
end
