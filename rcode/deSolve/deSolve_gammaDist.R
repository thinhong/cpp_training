library(deSolve)
library(ggplot2)
library(tidyr)

# Gamma and exponential distribution

sir_equations <- function(time, variables, parameters) {
  with(as.list(c(variables, parameters)), {
    dS <- -beta * (I1 + I2 + I3 + I4 + I5) * S
    dI1 <- (beta * (I1 + I2 + I3 + I4 + I5) * S) - (gamma * I1)
    dI2 <- (gamma * I1) - (gamma * I2)
    dI3 <- (gamma * I2) - (gamma * I3)
    dI4 <- (gamma * I3) - (gamma * I4)
    dI5 <- (gamma * I4) - (gamma * I5)
    dR <-  gamma * I5
    return(list(c(dS, dI1, dI2, dI3, dI4, dI5, dR)))
  })
}

parameters_values <- c(
  beta  = 0.0003, # infectious contact rate (/person/day)
  gamma = 0.2    # recovery rate (/day)
)

initial_values <- c(
  S = 999,  # number of susceptibles at time = 0
  I1 =  1,  # number of infectious at time = 0
  I2 = 0,
  I3 = 0,
  I4 = 0,
  I5 = 0,
  R =   0   # number of recovered (and immune) at time = 0
)

time_values <- seq(0, 100) # days

sir_values_1 <- ode(
  y = initial_values,
  times = time_values,
  func = sir_equations,
  parms = parameters_values
)

df <- as.data.frame(sir_values_1)
df$I <- rowSums(df[,grep("I", colnames(df))])
df <- df[,c("time", "S", "I", "R")]

df_plot <- gather(df, key = "Compartment", value = "Value", grep("time", colnames(df), invert = T))

ggplot(df_plot, aes(x = time, y = Value, col = Compartment)) +
  geom_line(size = 1) + 
  theme_minimal() +
  theme(text = element_text(size = 14))

