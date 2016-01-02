class Dice
  attr_reader :n_faces, :sides_weight
  def initialize(n_faces, sides_weight = [1] * n_faces)
    @n_faces = n_faces
    @sides_weight = sides_weight
  end

  def probabilities
    @sides_weight.collect do |weight|
      weight / @sides_weight.reduce(:+).to_f
    end
  end

  def honest?
    return @sides_weight == [1] * @sides_weight.length
  end

  def roll
    pos = 0
    random_number = Random.rand()

    probabilities.each_with_index do |probability, index|
      if pos + probability > random_number
        return index + 1
      else
        pos = pos + probability
      end
    end
  end
end
